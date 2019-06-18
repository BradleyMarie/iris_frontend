#include <iostream>
#include <stack>

#include "absl/strings/str_join.h"
#include "iris_physx_toolkit/constant_emissive_material.h"
#include "iris_physx_toolkit/constant_material.h"
#include "iris_physx_toolkit/kd_tree_scene.h"
#include "iris_physx_toolkit/lambertian_bsdf.h"
#include "iris_physx_toolkit/uniform_reflector.h"
#include "src/directive_parser.h"
#include "src/matrix_parser.h"
#include "src/ostream.h"
#include "src/param_matcher.h"
#include "src/scene_parser.h"
#include "src/shapes/parser.h"

namespace iris {
namespace {

class SingleSpectrumMatcher : public ParamMatcher {
 public:
  SingleSpectrumMatcher(const char* base_type_name, const char* type_name,
                        const char* parameter_name, Spectrum default_value)
      : ParamMatcher(base_type_name, type_name, parameter_name,
                     GetIndex<SpectrumParameter>()),
        m_value(default_value) {}
  const Spectrum& Get() { return m_value; }

 private:
  void Match(const ParameterData& data) final {
    if (absl::get<SpectrumParameter>(data).data.size() != 1) {
      NumberOfElementsError();
    }
    m_value = absl::get<SpectrumParameter>(data).data[0].first;
  }

 private:
  Spectrum m_value;
};

class SingleReflectorMatcher : public ParamMatcher {
 public:
  SingleReflectorMatcher(const char* base_type_name, const char* type_name,
                         const char* parameter_name, Reflector default_value)
      : ParamMatcher(base_type_name, type_name, parameter_name,
                     GetIndex<SpectrumParameter>()),
        m_value(default_value) {}
  const Reflector& Get() { return m_value; }

 private:
  void Match(const ParameterData& data) final {
    if (absl::get<SpectrumParameter>(data).data.size() != 1) {
      NumberOfElementsError();
    }
    if (!absl::get<SpectrumParameter>(data).data[0].second.get()) {
      std::cerr
          << "ERROR: Could not construct a reflection spectrum from values ("
          << absl::StrJoin(absl::get<SpectrumParameter>(data).values[0], ", ")
          << ")" << std::endl;
      exit(EXIT_FAILURE);
    }
    m_value = absl::get<SpectrumParameter>(data).data[0].second;
  }

 private:
  Reflector m_value;
};

struct AreaLightState {
  EmissiveMaterial emissive_material;
  bool two_sided;
};

class GraphicsStateManager {
 public:
  GraphicsStateManager();

  void TransformBegin(MatrixManager& matrix_manager);
  void TransformEnd(MatrixManager& matrix_manager);

  void AttributeBegin(MatrixManager& matrix_manager);
  void AttributeEnd(MatrixManager& matrix_manager);

  const absl::optional<AreaLightState>& GetAreaLightState();
  void SetAreaLightState(const AreaLightState& area_light,
                         const std::set<Spectrum> light_spectra);

  const absl::optional<Material>& GetMaterial();
  void SetMaterial(const Material& material,
                   const std::set<Reflector> material_reflectors);

  void PrecomputeColors(ColorIntegrator& color_integrator);

 private:
  struct ShaderState {
    absl::optional<AreaLightState> area_light;
    std::set<Spectrum> light_spectra;
    absl::optional<Material> material;
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
  m_shader_state.push({absl::nullopt, {}, absl::nullopt, {}});
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
  if (m_transform_state.empty() ||
      m_transform_state.top().push_reason != ATTRIBUTE) {
  }

  matrix_manager.RestoreState(m_transform_state.top().transforms,
                              m_transform_state.top().active_transforms);

  m_transform_state.pop();
  m_shader_state.pop();

  if (m_shader_state.empty()) {
    std::cerr << "ERROR: Mismatched AttributeBegin and AttributeEnd directives"
              << std::endl;
    exit(EXIT_FAILURE);
  }
}

const absl::optional<AreaLightState>&
GraphicsStateManager::GetAreaLightState() {
  m_spectra_used.insert(m_shader_state.top().light_spectra.begin(),
                        m_shader_state.top().light_spectra.end());
  return m_shader_state.top().area_light;
}

void GraphicsStateManager::SetAreaLightState(
    const AreaLightState& area_light, const std::set<Spectrum> light_spectra) {
  m_shader_state.top().area_light = area_light;
  m_shader_state.top().light_spectra = light_spectra;
}

const absl::optional<Material>& GraphicsStateManager::GetMaterial() {
  m_reflectors_used.insert(m_shader_state.top().material_reflectors.begin(),
                           m_shader_state.top().material_reflectors.end());
  return m_shader_state.top().material;
}

void GraphicsStateManager::SetMaterial(
    const Material& material, const std::set<Reflector> material_reflectors) {
  m_shader_state.top().material = material;
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

static const bool kDiffuseAreaLightDefaultTwoSided = false;
static const Spectrum kDiffuseAreaLightDefaultL;  // TODO: initialize

std::pair<AreaLightState, std::set<Spectrum>> ParseDiffuseAreaLight(
    const char* base_type_name, const char* type_name, Tokenizer& tokenizer) {
  SingleBoolMatcher twosided(base_type_name, type_name, "twosided",
                             kDiffuseAreaLightDefaultTwoSided);
  SingleSpectrumMatcher spectrum(base_type_name, type_name, "L",
                                 kDiffuseAreaLightDefaultL);
  ParseAllParameter<2>(base_type_name, type_name, tokenizer,
                       {&twosided, &spectrum});

  EmissiveMaterial emissive_material;
  ISTATUS status = ConstantEmissiveMaterialAllocate(
      (PSPECTRUM)spectrum.Get().get(),
      emissive_material.release_and_get_address());
  switch (status) {
    case ISTATUS_ALLOCATION_FAILED:
      std::cerr << "ERROR: Allocation failed" << std::endl;
      exit(EXIT_FAILURE);
    default:
      assert(status == ISTATUS_SUCCESS);
  }

  return std::make_pair(AreaLightState({emissive_material, twosided.Get()}),
                        std::set<Spectrum>({spectrum.Get()}));
}

static const float_t kMatteMaterialDefaultReflectance = (float_t)0.5;

std::pair<Material, std::set<Reflector>> ParseMatteMaterial(
    const char* base_type_name, const char* type_name, Tokenizer& tokenizer) {
  Reflector reflectance;
  ISTATUS status = UniformReflectorAllocate(
      kMatteMaterialDefaultReflectance, reflectance.release_and_get_address());
  switch (status) {
    case ISTATUS_ALLOCATION_FAILED:
      std::cerr << "ERROR: Allocation failed" << std::endl;
      exit(EXIT_FAILURE);
    default:
      assert(status == ISTATUS_SUCCESS);
  }

  SingleReflectorMatcher kd(base_type_name, type_name, "Kd", reflectance);
  ParseAllParameter<1>(base_type_name, type_name, tokenizer, {&kd});

  Bsdf bsdf;
  status = LambertianReflectorAllocate(kd.Get().get(),
                                       bsdf.release_and_get_address());
  switch (status) {
    case ISTATUS_ALLOCATION_FAILED:
      std::cerr << "ERROR: Allocation failed" << std::endl;
      exit(EXIT_FAILURE);
    default:
      assert(status == ISTATUS_SUCCESS);
  }

  Material result;
  status =
      ConstantMaterialAllocate(bsdf.get(), result.release_and_get_address());
  switch (status) {
    case ISTATUS_ALLOCATION_FAILED:
      std::cerr << "ERROR: Allocation failed" << std::endl;
      exit(EXIT_FAILURE);
    default:
      assert(status == ISTATUS_SUCCESS);
  }

  return std::make_pair(result, std::set<Reflector>({kd.Get()}));
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

template <typename Result>
using GraphicsDirectiveCallback = std::function<ShapeResult(
    const char* base_type_name, const char* type_name, Tokenizer&,
    MatrixManager&, GraphicsStateManager&)>;

template <typename Result>
DirectiveCallback<Result> BindCallback(
    GraphicsDirectiveCallback<Result> callback,
    GraphicsStateManager& graphics_manager) {
  return std::bind(callback, std::placeholders::_1, std::placeholders::_2,
                   std::placeholders::_3, std::placeholders::_4,
                   std::ref(graphics_manager));
}

template <typename Result>
using CallbackEntry = std::pair<const char*, DirectiveCallback<Result>>;

template <typename Result>
CallbackEntry<Result> CreateCallback(const char* type_name,
                                     GraphicsDirectiveCallback<Result> callback,
                                     GraphicsStateManager& graphics_manager) {
  return std::make_pair(type_name,
                        BindCallback<Result>(callback, graphics_manager));
}

}  // namespace

std::pair<Scene, std::vector<Light>> ParseScene(
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

    if (TryParseMatrix(*token, tokenizer, matrix_manager)) {
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
      auto light_state =
          ParseDirective<std::pair<AreaLightState, std::set<Spectrum>>, 1>(
              "AreaLightSource", tokenizer,
              {std::make_pair("diffuse", ParseDiffuseAreaLight)});
      graphics_state.SetAreaLightState(light_state.first, light_state.second);
      continue;
    }

    if (token == "Material") {
      auto material_state =
          ParseDirective<std::pair<Material, std::set<Reflector>>, 1>(
              "Material", tokenizer,
              {std::make_pair("matte", ParseMatteMaterial)});
      graphics_state.SetMaterial(material_state.first, material_state.second);
      continue;
    }

    if (token == "Shape") {
      absl::optional<EmissiveMaterial> front_emissive, back_emissive;
      if (graphics_state.GetAreaLightState()) {
        front_emissive = graphics_state.GetAreaLightState()->emissive_material;
        if (graphics_state.GetAreaLightState()->two_sided) {
          back_emissive = front_emissive;
        }
      }

      auto shape_result = ParseShape(
          "Shape", tokenizer, graphics_state.GetMaterial(),
          graphics_state.GetMaterial(), front_emissive, back_emissive);
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