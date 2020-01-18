#include "src/directives/geometry.h"

#include <iostream>
#include <list>
#include <stack>

#include "src/area_lights/parser.h"
#include "src/common/error.h"
#include "src/common/material_manager.h"
#include "src/common/named_material_manager.h"
#include "src/common/named_texture_manager.h"
#include "src/common/object_manager.h"
#include "src/common/texture_manager.h"
#include "src/directives/include.h"
#include "src/directives/transform.h"
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

  const std::pair<Material, Material>& GetMaterials();
  void SetMaterial(const Material& front_material,
                   const Material& back_material);

 private:
  struct ShaderState {
    std::pair<EmissiveMaterial, EmissiveMaterial> emissive_materials;
    std::pair<Material, Material> materials;
    NamedTextureManager named_texture_manager;
    NamedMaterialManager named_material_manager;
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
  m_shader_state.push(ShaderState());
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

const std::pair<Material, Material>& GraphicsStateManager::GetMaterials() {
  return m_shader_state.top().materials;
}

void GraphicsStateManager::SetMaterial(const Material& front_material,
                                       const Material& back_material) {
  m_shader_state.top().materials =
      std::make_pair(front_material, back_material);
}

}  // namespace

std::pair<Scene, std::vector<Light>> ParseGeometryDirectives(
    Tokenizer& tokenizer, MatrixManager& matrix_manager,
    SpectrumManager& spectrum_manager, ColorIntegrator& color_integrator) {
  matrix_manager.ActiveTransform(MatrixManager::ALL_TRANSFORMS);
  matrix_manager.Identity();

  std::vector<Light> lights;
  GraphicsStateManager graphics_state;
  MaterialManager material_manager;
  ObjectManager object_manager;
  TextureManager texture_manager;

  for (auto token = tokenizer.Next(); token; token = tokenizer.Next()) {
    if (token == "WorldEnd") {
      return std::make_pair(object_manager.AllocateScene(), std::move(lights));
    }

    if (TryParseTransformDirectives(*token, tokenizer, matrix_manager)) {
      continue;
    }

    if (TryParseInclude(*token, tokenizer)) {
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
      object_manager.ObjectBegin(tokenizer);
      continue;
    }

    if (token == "ObjectEnd") {
      object_manager.ObjectEnd();
      continue;
    }

    if (token == "ObjectInstance") {
      object_manager.ObjectInstance(tokenizer,
                                    matrix_manager.GetCurrent().first);
      continue;
    }

    if (token == "AreaLightSource") {
      auto light_state =
          ParseAreaLight("AreaLightSource", tokenizer, spectrum_manager);
      graphics_state.SetEmissiveMaterials(std::get<0>(light_state),
                                          std::get<1>(light_state));
      continue;
    }

    if (token == "Material") {
      auto material = ParseMaterial("Material", tokenizer, material_manager,
                                    graphics_state.GetNamedTextureManager(),
                                    texture_manager, spectrum_manager);
      graphics_state.SetMaterial(material, material);
      continue;
    }

    if (token == "MakeNamedMaterial") {
      auto material = ParseMakeNamedMaterial(
          "MakeNamedMaterial", tokenizer,
          graphics_state.GetNamedMaterialManager(), material_manager,
          graphics_state.GetNamedTextureManager(), texture_manager,
          spectrum_manager);
      graphics_state.SetMaterial(material, material);
      continue;
    }

    if (token == "NamedMaterial") {
      auto material = ParseNamedMaterial(
          "NamedMaterial", tokenizer, graphics_state.GetNamedMaterialManager());
      graphics_state.SetMaterial(material, material);
      continue;
    }

    if (token == "Shape") {
      auto materials = graphics_state.GetMaterials();
      auto emissive_materials = graphics_state.GetEmissiveMaterials();
      auto shape_result =
          ParseShape("Shape", tokenizer, materials.first, materials.second,
                     emissive_materials.first, emissive_materials.second);
      for (const auto& shape : shape_result.first) {
        object_manager.AddShape(shape, matrix_manager.GetCurrent().first,
                                emissive_materials.first,
                                emissive_materials.second);
      }
      for (const auto& light : shape_result.second) {
        lights.push_back(light);
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