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

class GeometryParser {
 public:
  static std::pair<Scene, std::vector<Light>> ParseGeometryDirectives(
      Tokenizer& tokenizer, MatrixManager& matrix_manager,
      SpectrumManager& spectrum_manager);

 private:
  GeometryParser(Tokenizer& tokenizer, MatrixManager& matrix_manager,
                 SpectrumManager& spectrum_manager)
      : m_tokenizer(tokenizer),
        m_matrix_manager(matrix_manager),
        m_spectrum_manager(spectrum_manager) {}

  bool ParseDirective(absl::string_view name, absl::string_view token,
                      void (GeometryParser::*implementation)(Directive&));
  void AreaLightSource(Directive& directive);
  void LightSource(Directive& directive);
  void MakeNamedMaterial(Directive& directive);
  void Material(Directive& directive);
  void NamedMaterial(Directive& directive);
  void ObjectBegin(Directive& directive);
  void ObjectInstance(Directive& directive);
  void ObjectEnd(Directive& directive);
  void Shape(Directive& directive);
  void Texture(Directive& directive);

  std::pair<Scene, std::vector<Light>> Parse();

  Tokenizer& m_tokenizer;
  MatrixManager& m_matrix_manager;
  SpectrumManager& m_spectrum_manager;
  GraphicsStateManager m_graphics_state;
  MaterialManager m_material_manager;
  NormalMapManager m_normal_map_manager;
  SceneBuilder m_scene_builder;
  TextureManager m_texture_manager;
};

bool GeometryParser::ParseDirective(
    absl::string_view name, absl::string_view token,
    void (GeometryParser::*implementation)(Directive&)) {
  if (token != name) {
    return false;
  }

  Directive directive(name, m_tokenizer);
  (this->*implementation)(directive);

  return true;
}

void GeometryParser::AreaLightSource(Directive& directive) {
  auto light_state = ParseAreaLight(directive, m_spectrum_manager);
  if (m_graphics_state.GetReverseOrientation()) {
    m_graphics_state.SetEmissiveMaterials(std::get<1>(light_state),
                                          std::get<0>(light_state));
  } else {
    m_graphics_state.SetEmissiveMaterials(std::get<0>(light_state),
                                          std::get<1>(light_state));
  }
}

void GeometryParser::LightSource(Directive& directive) {
  auto light = ParseLight(directive, m_spectrum_manager,
                          m_matrix_manager.GetCurrent().first);
  m_scene_builder.AddLight(light);
}

void GeometryParser::MakeNamedMaterial(Directive& directive) {
  auto name_and_material = ParseMakeNamedMaterial(
      directive, m_graphics_state.GetNamedTextureManager(),
      m_normal_map_manager, m_texture_manager, m_spectrum_manager);
  m_graphics_state.GetNamedMaterialManager().SetMaterial(
      name_and_material.first, name_and_material.second);
  m_graphics_state.SetMaterial(name_and_material.second);
}

void GeometryParser::Material(Directive& directive) {
  auto material = ParseMaterial(
      directive, m_graphics_state.GetNamedTextureManager(),
      m_normal_map_manager, m_texture_manager, m_spectrum_manager);
  m_graphics_state.SetMaterial(material);
}

void GeometryParser::NamedMaterial(Directive& directive) {
  auto name = ParseNamedMaterial(directive);
  auto material = m_graphics_state.GetNamedMaterialManager().GetMaterial(name);
  m_graphics_state.SetMaterial(material);
}

void GeometryParser::ObjectBegin(Directive& directive) {
  m_scene_builder.ObjectBegin(directive);
}

void GeometryParser::ObjectInstance(Directive& directive) {
  m_scene_builder.ObjectInstance(directive,
                                 m_matrix_manager.GetCurrent().first);
}

void GeometryParser::ObjectEnd(Directive& directive) {
  m_scene_builder.ObjectEnd(directive);
}

void GeometryParser::Shape(Directive& directive) {
  auto model_to_world = m_matrix_manager.GetCurrent().first;
  auto material = m_graphics_state.GetMaterials();
  auto emissive_materials = m_graphics_state.GetEmissiveMaterials();
  auto shape_result =
      ParseShape(directive, model_to_world, m_material_manager,
                 m_graphics_state.GetNamedTextureManager(),
                 m_normal_map_manager, m_texture_manager, m_spectrum_manager,
                 material, emissive_materials.first, emissive_materials.second);
  if (std::get<2>(shape_result) == ShapeCoordinateSystem::World) {
    model_to_world.reset();
  }
  for (const auto& shape : std::get<0>(shape_result)) {
    m_scene_builder.AddShape(shape, model_to_world);
  }
  for (const auto& light : std::get<1>(shape_result)) {
    m_scene_builder.AddAreaLight(std::get<0>(light), model_to_world,
                                 std::get<1>(light), std::get<2>(light));
  }
}

void GeometryParser::Texture(Directive& directive) {
  ParseTexture(directive, m_graphics_state.GetNamedTextureManager(),
               m_texture_manager, m_spectrum_manager);
}

std::pair<Scene, std::vector<Light>> GeometryParser::Parse() {
  m_matrix_manager.ActiveTransform(MatrixManager::ALL_TRANSFORMS);
  m_matrix_manager.Identity();

  for (auto token = m_tokenizer.Next(); token; token = m_tokenizer.Next()) {
    if (token == "WorldEnd") {
      return m_scene_builder.Build();
    }

    if (TryParseTransformDirectives(*token, m_tokenizer, m_matrix_manager)) {
      continue;
    }

    if (TryParseInclude(*token, m_tokenizer)) {
      continue;
    }

    if (token == "ReverseOrientation") {
      m_graphics_state.FlipReverseOrientation();
      continue;
    }

    if (token == "AttributeBegin") {
      m_graphics_state.AttributeBegin(m_matrix_manager);
      continue;
    }

    if (token == "AttributeEnd") {
      m_graphics_state.AttributeEnd(m_matrix_manager);
      continue;
    }

    if (token == "TransformBegin") {
      m_graphics_state.TransformBegin(m_matrix_manager);
      continue;
    }

    if (token == "TransformEnd") {
      m_graphics_state.TransformEnd(m_matrix_manager);
      continue;
    }

    if (ParseDirective("ObjectBegin", *token, &GeometryParser::ObjectBegin)) {
      continue;
    }

    if (ParseDirective("ObjectEnd", *token, &GeometryParser::ObjectEnd)) {
      continue;
    }

    if (ParseDirective("ObjectInstance", *token,
                       &GeometryParser::ObjectInstance)) {
      continue;
    }

    if (ParseDirective("AreaLightSource", *token,
                       &GeometryParser::AreaLightSource)) {
      continue;
    }

    if (ParseDirective("LightSource", *token, &GeometryParser::LightSource)) {
      continue;
    }

    if (ParseDirective("Material", *token, &GeometryParser::Material)) {
      continue;
    }

    if (ParseDirective("MakeNamedMaterial", *token,
                       &GeometryParser::MakeNamedMaterial)) {
      continue;
    }

    if (ParseDirective("NamedMaterial", *token,
                       &GeometryParser::NamedMaterial)) {
      continue;
    }

    if (ParseDirective("Shape", *token, &GeometryParser::Shape)) {
      continue;
    }

    if (token == "MakeNamedMedium") {
      std::cerr << "ERROR: Invalid directive after WorldBegin: " << *token
                << std::endl;
      exit(EXIT_FAILURE);
      continue;
    }

    if (token == "MediumInterface") {
      std::cerr << "ERROR: Invalid directive after WorldBegin: " << *token
                << std::endl;
      exit(EXIT_FAILURE);
      continue;
    }

    if (ParseDirective("Texture", *token, &GeometryParser::Texture)) {
      continue;
    }

    if (token == "TransformTimes") {
      std::cerr << "ERROR: Invalid directive after WorldBegin: " << *token
                << std::endl;
      exit(EXIT_FAILURE);
      continue;
    }

    std::cerr << "ERROR: Invalid directive after WorldEnd: " << *token
              << std::endl;
    exit(EXIT_FAILURE);
  }

  std::cerr << "ERROR: Missing WorldEnd directive" << std::endl;
  exit(EXIT_FAILURE);
}

std::pair<Scene, std::vector<Light>> GeometryParser::ParseGeometryDirectives(
    Tokenizer& tokenizer, MatrixManager& matrix_manager,
    SpectrumManager& spectrum_manager) {
  GeometryParser parser(tokenizer, matrix_manager, spectrum_manager);
  return parser.Parse();
}

}  // namespace

std::pair<Scene, std::vector<Light>> ParseGeometryDirectives(
    Tokenizer& tokenizer, MatrixManager& matrix_manager,
    SpectrumManager& spectrum_manager) {
  return GeometryParser::ParseGeometryDirectives(tokenizer, matrix_manager,
                                                 spectrum_manager);
}

}  // namespace iris