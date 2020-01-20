#include "src/directives/geometry.h"

#include <iostream>
#include <list>
#include <stack>

#include "src/area_lights/parser.h"
#include "src/common/error.h"
#include "src/common/material_manager.h"
#include "src/common/named_material_manager.h"
#include "src/common/named_texture_manager.h"
#include "src/common/texture_manager.h"
#include "src/directives/include.h"
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
    SpectrumManager& spectrum_manager, ColorIntegrator& color_integrator) {
  matrix_manager.ActiveTransform(MatrixManager::ALL_TRANSFORMS);
  matrix_manager.Identity();

  GraphicsStateManager graphics_state;
  MaterialManager material_manager;
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
      scene_builder.ObjectBegin(tokenizer);
      continue;
    }

    if (token == "ObjectEnd") {
      scene_builder.ObjectEnd();
      continue;
    }

    if (token == "ObjectInstance") {
      scene_builder.ObjectInstance(tokenizer,
                                   matrix_manager.GetCurrent().first);
      continue;
    }

    if (token == "AreaLightSource") {
      auto light_state =
          ParseAreaLight("AreaLightSource", tokenizer, spectrum_manager);
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
      auto light = ParseLight("LightSource", tokenizer, spectrum_manager,
                              matrix_manager.GetCurrent().first);
      scene_builder.AddLight(light);
      continue;
    }

    if (token == "Material") {
      auto material = ParseMaterial("Material", tokenizer,
                                    graphics_state.GetNamedTextureManager(),
                                    texture_manager, spectrum_manager);
      graphics_state.SetMaterial(material);
      continue;
    }

    if (token == "MakeNamedMaterial") {
      auto material =
          ParseMakeNamedMaterial("MakeNamedMaterial", tokenizer,
                                 graphics_state.GetNamedMaterialManager(),
                                 graphics_state.GetNamedTextureManager(),
                                 texture_manager, spectrum_manager);
      graphics_state.SetMaterial(material);
      continue;
    }

    if (token == "NamedMaterial") {
      auto material = ParseNamedMaterial(
          "NamedMaterial", tokenizer, graphics_state.GetNamedMaterialManager());
      graphics_state.SetMaterial(material);
      continue;
    }

    if (token == "Shape") {
      auto material = graphics_state.GetMaterials();
      auto emissive_materials = graphics_state.GetEmissiveMaterials();
      auto shape_result =
          ParseShape("Shape", tokenizer, material_manager,
                     graphics_state.GetNamedTextureManager(), texture_manager,
                     spectrum_manager, material, emissive_materials.first,
                     emissive_materials.second);
      for (const auto& shape : shape_result.first) {
        scene_builder.AddShape(shape, matrix_manager.GetCurrent().first);
      }
      for (const auto& light : shape_result.second) {
        scene_builder.AddAreaLight(std::get<0>(light),
                                   matrix_manager.GetCurrent().first,
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
      ParseTexture("Texture", tokenizer,
                   graphics_state.GetNamedTextureManager(), texture_manager,
                   spectrum_manager);
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