#include "src/directives/geometry.h"

#include <iostream>
#include <stack>

#include "iris_physx_toolkit/kd_tree_scene.h"
#include "src/area_lights/parser.h"
#include "src/directives/transform.h"
#include "src/materials/parser.h"
#include "src/shapes/parser.h"

namespace iris {
namespace {

class GraphicsStateManager {
 public:
  GraphicsStateManager();

  void TransformBegin(MatrixManager& matrix_manager);
  void TransformEnd(MatrixManager& matrix_manager);

  void AttributeBegin(MatrixManager& matrix_manager);
  void AttributeEnd(MatrixManager& matrix_manager);

  const std::pair<EmissiveMaterial, EmissiveMaterial>& GetEmissiveMaterials();
  void SetEmissiveMaterials(const EmissiveMaterial& front_emissive_material,
                            const EmissiveMaterial& back_emissive_material,
                            const std::set<Spectrum> light_spectra);

  const std::pair<Material, Material>& GetMaterials();
  void SetMaterial(const Material& front_material,
                   const Material& back_material,
                   const std::set<Reflector> material_reflectors);

  void PrecomputeColors(ColorIntegrator& color_integrator);

 private:
  struct ShaderState {
    std::pair<EmissiveMaterial, EmissiveMaterial> emissive_materials;
    std::pair<Material, Material> materials;
    std::set<Spectrum> light_spectra;
    std::set<Reflector> material_reflectors;
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

  std::set<Spectrum> m_spectra_used;
  std::set<Reflector> m_reflectors_used;
  std::stack<ShaderState> m_shader_state;
  std::stack<TransformState> m_transform_state;
  std::map<std::string, Material> m_named_materials;
  std::map<std::string, std::vector<Shape>> m_named_objects;
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

const std::pair<EmissiveMaterial, EmissiveMaterial>&
GraphicsStateManager::GetEmissiveMaterials() {
  m_spectra_used.insert(m_shader_state.top().light_spectra.begin(),
                        m_shader_state.top().light_spectra.end());
  return m_shader_state.top().emissive_materials;
}

void GraphicsStateManager::SetEmissiveMaterials(
    const EmissiveMaterial& front_emissive_material,
    const EmissiveMaterial& back_emissive_material,
    const std::set<Spectrum> light_spectra) {
  m_shader_state.top().emissive_materials =
      std::make_pair(front_emissive_material, back_emissive_material);
  m_shader_state.top().light_spectra = light_spectra;
}

const std::pair<Material, Material>& GraphicsStateManager::GetMaterials() {
  m_reflectors_used.insert(m_shader_state.top().material_reflectors.begin(),
                           m_shader_state.top().material_reflectors.end());
  return m_shader_state.top().materials;
}

void GraphicsStateManager::SetMaterial(
    const Material& front_material, const Material& back_material,
    const std::set<Reflector> material_reflectors) {
  m_shader_state.top().materials =
      std::make_pair(front_material, back_material);
  m_shader_state.top().material_reflectors = material_reflectors;
}

void GraphicsStateManager::PrecomputeColors(ColorIntegrator& color_integrator) {
  for (const auto& spectrum : m_spectra_used) {
    ISTATUS status = ColorIntegratorPrecomputeSpectrumColor(
        color_integrator.get(), spectrum.get());
    switch (status) {
      case ISTATUS_ALLOCATION_FAILED:
        std::cerr << "ERROR: Allocation failed" << std::endl;
        exit(EXIT_FAILURE);
      default:
        assert(status == ISTATUS_SUCCESS);
    }
  }
  for (const auto& reflector : m_reflectors_used) {
    ISTATUS status = ColorIntegratorPrecomputeReflectorColor(
        color_integrator.get(), reflector.get());
    switch (status) {
      case ISTATUS_ALLOCATION_FAILED:
        std::cerr << "ERROR: Allocation failed" << std::endl;
        exit(EXIT_FAILURE);
      default:
        assert(status == ISTATUS_SUCCESS);
    }
  }
}

Scene CreateScene(std::vector<Shape>& shapes, std::vector<Matrix>& transforms) {
  assert(shapes.size() == transforms.size());
  std::vector<PSHAPE> shape_pointers;
  std::vector<PMATRIX> matrix_pointers;
  std::unique_ptr<bool[]> premultiplied(new bool[shapes.size()]);

  for (size_t i = 0; i < shapes.size(); i++) {
    shape_pointers.push_back(shapes[i].get());
    matrix_pointers.push_back(transforms[i].get());
    premultiplied[i] = false;
  }

  Scene result;
  ISTATUS status = KdTreeSceneAllocate(
      shape_pointers.data(), matrix_pointers.data(), premultiplied.get(),
      shapes.size(), result.release_and_get_address());
  switch (status) {
    case ISTATUS_ALLOCATION_FAILED:
      std::cerr << "ERROR: Allocation failed" << std::endl;
      exit(EXIT_FAILURE);
    default:
      assert(status == ISTATUS_SUCCESS);
  }

  return result;
}

}  // namespace

std::pair<Scene, std::vector<Light>> ParseGeometryDirectives(
    Tokenizer& tokenizer, MatrixManager& matrix_manager,
    ColorIntegrator& color_integrator) {
  matrix_manager.ActiveTransform(MatrixManager::ALL_TRANSFORMS);
  matrix_manager.Identity();

  std::vector<Shape> shapes;
  std::vector<Matrix> transforms;
  std::vector<Light> lights;
  GraphicsStateManager graphics_state;

  for (auto token = tokenizer.Next(); token; token = tokenizer.Next()) {
    if (token == "WorldEnd") {
      graphics_state.PrecomputeColors(color_integrator);
      return std::make_pair(CreateScene(shapes, transforms), std::move(lights));
    }

    if (TryParseTransformDirectives(*token, tokenizer, matrix_manager)) {
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

    if (token == "AreaLightSource") {
      auto light_state = ParseAreaLight("AreaLightSource", tokenizer);
      graphics_state.SetEmissiveMaterials(std::get<0>(light_state),
                                          std::get<1>(light_state),
                                          std::get<2>(light_state));
      continue;
    }

    if (token == "Material") {
      auto material_state = ParseMaterial("Material", tokenizer);
      graphics_state.SetMaterial(material_state.first, material_state.first,
                                 material_state.second);
      continue;
    }

    if (token == "Shape") {
      auto materials = graphics_state.GetMaterials();
      auto emissive_materials = graphics_state.GetEmissiveMaterials();
      auto shape_result =
          ParseShape("Shape", tokenizer, materials.first, materials.second,
                     emissive_materials.first, emissive_materials.second);
      for (const auto& shape : shape_result.first) {
        shapes.push_back(shape);
        transforms.push_back(matrix_manager.GetCurrent().first);
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