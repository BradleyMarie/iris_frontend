#include "src/directives/geometry.h"

#include <iostream>
#include <list>
#include <stack>

#include "src/area_lights/parser.h"
#include "src/common/error.h"
#include "src/common/material_manager.h"
#include "src/common/named_texture_manager.h"
#include "src/common/normal_map_manager.h"
#include "src/common/texture_manager.h"
#include "src/directives/include.h"
#include "src/directives/named_material_manager.h"
#include "src/directives/scene_builder.h"
#include "src/directives/transform.h"
#include "src/lights/parser.h"
#include "src/materials/parser.h"
#include "src/shapes/parser.h"
#include "src/textures/parser.h"

namespace iris {
namespace {

class GraphicsStateManager {
 public:
  GraphicsStateManager();

  void TransformBegin(MatrixManager& matrix_manager);
  void TransformEnd(MatrixManager& matrix_manager);

  void AttributeBegin(MatrixManager& matrix_manager);
  void AttributeEnd(MatrixManager& matrix_manager);

  NamedTextureManager& GetNamedTextureManager();
  NamedMaterialManager& GetNamedMaterialManager();

  const std::pair<EmissiveMaterial, EmissiveMaterial>& GetEmissiveMaterials();
  void SetEmissiveMaterials(const EmissiveMaterial& front_emissive_material,
                            const EmissiveMaterial& back_emissive_material);

  const MaterialResult& GetMaterials();
  void SetMaterial(const MaterialResult& material);

  bool GetReverseOrientation() const;
  void FlipReverseOrientation();

 private:
  struct ShaderState {
    std::pair<EmissiveMaterial, EmissiveMaterial> emissive_materials;
    MaterialResult material;
    NamedTextureManager named_texture_manager;
    NamedMaterialManager named_material_manager;
    bool reverse_orientation;
  };

  enum PushReason {
    TRANSFORM = 1,
    ATTRIBUTE = 2,
  };

  struct TransformState {
    std::pair<Matrix, Matrix> transforms;
    MatrixManager::Active active_transforms;
    PushReason push_reason;
  };

  std::stack<ShaderState, std::list<ShaderState>> m_shader_state;
  std::stack<TransformState, std::list<TransformState>> m_transform_state;
};

GraphicsStateManager::GraphicsStateManager() {
  ShaderState shader_state;
  shader_state.reverse_orientation = false;
  shader_state.material =
      [](Parameters& parameters, MaterialManager& material_manager,
         const NamedTextureManager& named_texture_manager, NormalMapManager&,
         TextureManager& texture_manager,
         SpectrumManager& spectrum_manager) -> std::pair<Material, NormalMap> {
    parameters.Match();
    return std::make_pair(Material(), NormalMap());
  };
  m_shader_state.push(shader_state);
}

void GraphicsStateManager::TransformBegin(MatrixManager& matrix_manager) {
  m_transform_state.push(
      {matrix_manager.GetCurrent(), matrix_manager.GetActive(), TRANSFORM});
}

void GraphicsStateManager::TransformEnd(MatrixManager& matrix_manager) {
  if (m_transform_state.empty() ||
      m_transform_state.top().push_reason != TRANSFORM) {
    std::cerr << "ERROR: Mismatched TransformBegin and TransformEnd directives"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  matrix_manager.RestoreState(m_transform_state.top().transforms,
                              m_transform_state.top().active_transforms);

  m_transform_state.pop();
}

void GraphicsStateManager::AttributeBegin(MatrixManager& matrix_manager) {
  m_transform_state.push(
      {matrix_manager.GetCurrent(), matrix_manager.GetActive(), ATTRIBUTE});
  m_shader_state.push(m_shader_state.top());
}

void GraphicsStateManager::AttributeEnd(MatrixManager& matrix_manager) {
  if (m_shader_state.size() == 1 || m_transform_state.empty() ||
      m_transform_state.top().push_reason != ATTRIBUTE) {
    std::cerr << "ERROR: Mismatched AttributeBegin and AttributeEnd directives"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  matrix_manager.RestoreState(m_transform_state.top().transforms,
                              m_transform_state.top().active_transforms);

  m_transform_state.pop();
  m_shader_state.pop();
}

NamedTextureManager& GraphicsStateManager::GetNamedTextureManager() {
  return m_shader_state.top().named_texture_manager;
}

NamedMaterialManager& GraphicsStateManager::GetNamedMaterialManager() {
  return m_shader_state.top().named_material_manager;
}

const std::pair<EmissiveMaterial, EmissiveMaterial>&
GraphicsStateManager::GetEmissiveMaterials() {
  return m_shader_state.top().emissive_materials;
}

void GraphicsStateManager::SetEmissiveMaterials(
    const EmissiveMaterial& front_emissive_material,
    const EmissiveMaterial& back_emissive_material) {
  m_shader_state.top().emissive_materials =
      std::make_pair(front_emissive_material, back_emissive_material);
}

const MaterialResult& GraphicsStateManager::GetMaterials() {
  return m_shader_state.top().material;
}

void GraphicsStateManager::SetMaterial(const MaterialResult& material) {
  m_shader_state.top().material = material;
}

bool GraphicsStateManager::GetReverseOrientation() const {
  return m_shader_state.top().reverse_orientation;
}

void GraphicsStateManager::FlipReverseOrientation() {
  m_shader_state.top().reverse_orientation =
      !m_shader_state.top().reverse_orientation;
}

}  // namespace

std::pair<Scene, std::vector<Light>> ParseGeometryDirectives(
    Tokenizer& tokenizer, MatrixManager& matrix_manager,
    SpectrumManager& spectrum_manager) {
  matrix_manager.ActiveTransform(MatrixManager::ALL_TRANSFORMS);
  matrix_manager.Identity();

  GraphicsStateManager graphics_state;
  MaterialManager material_manager;
  NormalMapManager normal_map_manager;
  SceneBuilder scene_builder;
  TextureManager texture_manager;

  for (auto token = tokenizer.Next(); token; token = tokenizer.Next()) {
    if (token == "WorldEnd") {
      return scene_builder.Build();
    }

    if (TryParseTransformDirectives(*token, tokenizer, matrix_manager)) {
      continue;
    }

    if (TryParseInclude(*token, tokenizer)) {
      continue;
    }

    if (token == "ReverseOrientation") {
      graphics_state.FlipReverseOrientation();
      continue;
    }

    if (token == "AttributeBegin") {
      graphics_state.AttributeBegin(matrix_manager);
      continue;
    }

    if (token == "AttributeEnd") {
      graphics_state.AttributeEnd(matrix_manager);
      continue;
    }

    if (token == "TransformBegin") {
      graphics_state.TransformBegin(matrix_manager);
      continue;
    }

    if (token == "TransformEnd") {
      graphics_state.TransformEnd(matrix_manager);
      continue;
    }

    if (token == "ObjectBegin") {
      Directive directive("ObjectBegin", tokenizer);
      scene_builder.ObjectBegin(directive);
      continue;
    }

    if (token == "ObjectEnd") {
      Directive directive("ObjectEnd", tokenizer);
      scene_builder.ObjectEnd(directive);
      continue;
    }

    if (token == "ObjectInstance") {
      Directive directive("ObjectInstance", tokenizer);
      scene_builder.ObjectInstance(directive,
                                   matrix_manager.GetCurrent().first);
      continue;
    }

    if (token == "AreaLightSource") {
      Directive directive("AreaLightSource", tokenizer);
      auto light_state = ParseAreaLight(directive, spectrum_manager);
      if (graphics_state.GetReverseOrientation()) {
        graphics_state.SetEmissiveMaterials(std::get<1>(light_state),
                                            std::get<0>(light_state));
      } else {
        graphics_state.SetEmissiveMaterials(std::get<0>(light_state),
                                            std::get<1>(light_state));
      }
      continue;
    }

    if (token == "LightSource") {
      Directive directive("LightSource", tokenizer);
      auto light = ParseLight(directive, spectrum_manager,
                              matrix_manager.GetCurrent().first);
      scene_builder.AddLight(light);
      continue;
    }

    if (token == "Material") {
      Directive directive("Material", tokenizer);
      auto material = ParseMaterial(
          directive, graphics_state.GetNamedTextureManager(),
          normal_map_manager, texture_manager, spectrum_manager);
      graphics_state.SetMaterial(material);
      continue;
    }

    if (token == "MakeNamedMaterial") {
      Directive directive("MakeNamedMaterial", tokenizer);
      auto name_and_material = ParseMakeNamedMaterial(
          directive, graphics_state.GetNamedTextureManager(),
          normal_map_manager, texture_manager, spectrum_manager);
      graphics_state.GetNamedMaterialManager().SetMaterial(
          name_and_material.first, name_and_material.second);
      graphics_state.SetMaterial(name_and_material.second);
      continue;
    }

    if (token == "NamedMaterial") {
      Directive directive("NamedMaterial", tokenizer);
      auto name = ParseNamedMaterial(directive);
      auto material =
          graphics_state.GetNamedMaterialManager().GetMaterial(name);
      graphics_state.SetMaterial(material);
      continue;
    }

    if (token == "Shape") {
      auto model_to_world = matrix_manager.GetCurrent().first;
      auto material = graphics_state.GetMaterials();
      auto emissive_materials = graphics_state.GetEmissiveMaterials();
      Directive directive("Shape", tokenizer);
      auto shape_result = ParseShape(
          directive, model_to_world, material_manager,
          graphics_state.GetNamedTextureManager(), normal_map_manager,
          texture_manager, spectrum_manager, material, emissive_materials.first,
          emissive_materials.second);
      if (std::get<2>(shape_result) == ShapeCoordinateSystem::World) {
        model_to_world.reset();
      }
      for (const auto& shape : std::get<0>(shape_result)) {
        scene_builder.AddShape(shape, model_to_world);
      }
      for (const auto& light : std::get<1>(shape_result)) {
        scene_builder.AddAreaLight(std::get<0>(light), model_to_world,
                                   std::get<1>(light), std::get<2>(light));
      }
      continue;
    }

    if (token == "MakeNamedMedium") {
      std::cerr << "ERROR: Invalid directive before WorldBegin: " << *token
                << std::endl;
      exit(EXIT_FAILURE);
      continue;
    }

    if (token == "MediumInterface") {
      std::cerr << "ERROR: Invalid directive before WorldBegin: " << *token
                << std::endl;
      exit(EXIT_FAILURE);
      continue;
    }

    if (token == "Texture") {
      Directive directive("Texture", tokenizer);
      ParseTexture(directive, graphics_state.GetNamedTextureManager(),
                   texture_manager, spectrum_manager);
      continue;
    }

    if (token == "TransformTimes") {
      std::cerr << "ERROR: Invalid directive before WorldBegin: " << *token
                << std::endl;
      exit(EXIT_FAILURE);
      continue;
    }

    std::cerr << "ERROR: Invalid directive before WorldEnd: " << *token
              << std::endl;
    exit(EXIT_FAILURE);
  }

  std::cerr << "ERROR: Missing WorldEnd directive" << std::endl;
  exit(EXIT_FAILURE);
}

}  // namespace iris