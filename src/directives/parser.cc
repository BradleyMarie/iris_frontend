#include "src/directives/parser.h"

#include <iostream>
#include <set>
#include <stack>
#include <vector>

#include "absl/strings/str_join.h"
#include "iris_physx_toolkit/color_spectra.h"
#include "iris_physx_toolkit/reflective_color_integrator.h"
#include "src/area_lights/parser.h"
#include "src/cameras/parser.h"
#include "src/color_extrapolators/parser.h"
#include "src/color_integrators/parser.h"
#include "src/common/error.h"
#include "src/common/material_manager.h"
#include "src/common/named_texture_manager.h"
#include "src/common/normal_map_manager.h"
#include "src/common/quoted_string.h"
#include "src/common/spectrum_manager.h"
#include "src/common/texture_manager.h"
#include "src/directives/named_material_manager.h"
#include "src/directives/scene_builder.h"
#include "src/directives/spectral_representation_parser.h"
#include "src/films/parser.h"
#include "src/integrators/parser.h"
#include "src/lights/parser.h"
#include "src/materials/parser.h"
#include "src/randoms/parser.h"
#include "src/samplers/parser.h"
#include "src/shapes/parser.h"
#include "src/textures/parser.h"

namespace iris {
namespace {

bool TryParseInclude(absl::string_view directive, Tokenizer& tokenizer) {
  if (directive != "Include") {
    return false;
  }

  auto token = tokenizer.Next();
  if (!token) {
    std::cerr << "ERROR: Include requires 1 parameter" << std::endl;
    exit(EXIT_FAILURE);
  }

  auto unquoted = UnquoteToken(*token);
  if (!unquoted) {
    std::cerr << "ERROR: Failed to parse Include parameter: " << *token
              << std::endl;
    exit(EXIT_FAILURE);
  }

  std::string as_string(unquoted->data(), unquoted->size());
  tokenizer.Include(as_string);

  return true;
}

class MatrixManager {
 public:
  MatrixManager();

  enum Active {
    START_TRANSFORM = 1,
    END_TRANSFORM = 2,
    ALL_TRANSFORMS = 3,
  };

  bool Parse(absl::string_view token, Tokenizer& tokenizer);

  void Reset();
  Active GetActive() const { return m_active; }
  const std::pair<Matrix, Matrix>& GetCurrent();
  void RestoreState(const std::pair<Matrix, Matrix>& transforms,
                    Active active) {
    m_current = transforms;
    m_active = active;
  }

 private:
  bool ParseDirective(absl::string_view name, absl::string_view token,
                      Tokenizer& tokenizer,
                      void (MatrixManager::*implementation)(Directive&));

  void Identity(Directive& directive);
  void Translate(Directive& directive);
  void Scale(Directive& directive);
  void Rotate(Directive& directive);
  void LookAt(Directive& directive);
  void CoordinateSystem(Directive& directive);
  void CoordSysTransform(Directive& directive);
  void Transform(Directive& directive);
  void ConcatTransform(Directive& directive);
  void ActiveTransform(Directive& directive);

  void Transform(Matrix m);
  void Set(Matrix m);

  class Cache {
   public:
    Matrix Lookup(Matrix matrix) {
      float_t contents[4][4];
      MatrixReadContents(matrix.get(), contents);
      if (contents[0][0] == (float_t)1.0 && contents[0][1] == (float_t)0.0 &&
          contents[0][2] == (float_t)0.0 && contents[0][3] == (float_t)0.0 &&
          contents[1][0] == (float_t)0.0 && contents[1][1] == (float_t)1.0 &&
          contents[1][2] == (float_t)0.0 && contents[1][3] == (float_t)0.0 &&
          contents[2][0] == (float_t)0.0 && contents[2][1] == (float_t)0.0 &&
          contents[2][2] == (float_t)1.0 && contents[2][3] == (float_t)0.0 &&
          contents[3][0] == (float_t)0.0 && contents[3][1] == (float_t)0.0 &&
          contents[3][2] == (float_t)0.0 && contents[3][3] == (float_t)1.0) {
        return Matrix();
      }

      auto it = m_transforms.find(matrix);
      if (it != m_transforms.end()) {
        return *it;
      }

      Matrix inverse;
      *inverse.release_and_get_address() = MatrixGetInverse(matrix.get());

      m_transforms.insert(matrix);
      m_transforms.insert(inverse);

      return matrix;
    }

   private:
    struct Comparator {
      bool operator()(const Matrix& lhs, const Matrix& rhs) const {
        float_t l_contents[4][4];
        MatrixReadContents(lhs.get(), l_contents);

        float_t r_contents[4][4];
        MatrixReadContents(rhs.get(), r_contents);

        for (size_t i = 0; i < 4; i++) {
          for (size_t j = 0; j < 4; j++) {
            if (l_contents[i][j] != r_contents[i][j]) {
              return l_contents[i][j] < r_contents[i][j];
            }
          }
        }

        return false;
      }
    };

    std::set<Matrix, Comparator> m_transforms;
  };

  Active m_active;
  absl::flat_hash_map<std::string, std::pair<Matrix, Matrix>>
      m_coordinate_systems;
  std::pair<Matrix, Matrix> m_current;
  Cache m_cache;
};

MatrixManager::MatrixManager() : m_active(ALL_TRANSFORMS) {}

const std::pair<Matrix, Matrix>& MatrixManager::GetCurrent() {
  m_current.first = m_cache.Lookup(m_current.first);
  m_current.second = m_cache.Lookup(m_current.second);
  return m_current;
}

void MatrixManager::Reset() {
  m_active = MatrixManager::ALL_TRANSFORMS;
  Set(Matrix());
}

bool MatrixManager::Parse(absl::string_view token, Tokenizer& tokenizer) {
  if (ParseDirective("Identity", token, tokenizer, &MatrixManager::Identity)) {
    return true;
  }

  if (ParseDirective("Translate", token, tokenizer,
                     &MatrixManager::Translate)) {
    return true;
  }

  if (ParseDirective("Scale", token, tokenizer, &MatrixManager::Scale)) {
    return true;
  }

  if (ParseDirective("Rotate", token, tokenizer, &MatrixManager::Rotate)) {
    return true;
  }

  if (ParseDirective("LookAt", token, tokenizer, &MatrixManager::LookAt)) {
    return true;
  }

  if (ParseDirective("CoordinateSystem", token, tokenizer,
                     &MatrixManager::CoordinateSystem)) {
    return true;
  }

  if (ParseDirective("CoordSysTransform", token, tokenizer,
                     &MatrixManager::CoordSysTransform)) {
    return true;
  }

  if (ParseDirective("Transform", token, tokenizer,
                     &MatrixManager::Transform)) {
    return true;
  }

  if (ParseDirective("ConcatTransform", token, tokenizer,
                     &MatrixManager::ConcatTransform)) {
    return true;
  }

  if (ParseDirective("ActiveTransform", token, tokenizer,
                     &MatrixManager::ActiveTransform)) {
    return true;
  }

  return false;
}

bool MatrixManager::ParseDirective(
    absl::string_view name, absl::string_view token, Tokenizer& tokenizer,
    void (MatrixManager::*implementation)(Directive&)) {
  if (token != name) {
    return false;
  }

  Directive directive(name, tokenizer);
  (this->*implementation)(directive);

  return true;
}

void MatrixManager::Identity(Directive& directive) {
  directive.Empty();
  Set(Matrix());
}

void MatrixManager::Translate(Directive& directive) {
  std::array<float_t, 3> params;
  directive.FiniteFloats(absl::MakeSpan(params), absl::nullopt);

  Matrix scalar;
  ISTATUS status = MatrixAllocateTranslation(params[0], params[1], params[2],
                                             scalar.release_and_get_address());
  SuccessOrOOM(status);

  Transform(scalar);
}

void MatrixManager::Scale(Directive& directive) {
  std::array<float_t, 3> params;
  directive.FiniteFloats(absl::MakeSpan(params), absl::nullopt);

  Matrix scalar;
  ISTATUS status = MatrixAllocateScalar(params[0], params[1], params[2],
                                        scalar.release_and_get_address());

  switch (status) {
    case ISTATUS_INVALID_ARGUMENT_00:
      std::cerr << "ERROR: The x parameters of Scale must be non-zero"
                << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_INVALID_ARGUMENT_01:
      std::cerr << "ERROR: The y parameters of Scale must be non-zero"
                << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_INVALID_ARGUMENT_02:
      std::cerr << "ERROR: The z parameters of Scale must be non-zero"
                << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_ALLOCATION_FAILED:
      ReportOOM();
    default:
      assert(status == ISTATUS_SUCCESS);
  }

  Transform(scalar);
}

void MatrixManager::Rotate(Directive& directive) {
  std::array<float_t, 4> params;
  directive.FiniteFloats(absl::MakeSpan(params), absl::nullopt);

  const float_t to_radians =
      (float_t)3.1415926535897932384626433832 / (float_t)180.0;

  Matrix rotation;
  ISTATUS status =
      MatrixAllocateRotation(params[0] * to_radians, params[1], params[2],
                             params[3], rotation.release_and_get_address());

  switch (status) {
    case ISTATUS_INVALID_ARGUMENT_COMBINATION_00:
      std::cerr << "ERROR: One of the x, y, or z parameters of Rotate must be "
                   "non-zero"
                << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_ALLOCATION_FAILED:
      ReportOOM();
    default:
      assert(status == ISTATUS_SUCCESS);
  }

  Transform(rotation);
}

void MatrixManager::LookAt(Directive& directive) {
  std::array<float_t, 9> params;
  directive.FiniteFloats(absl::MakeSpan(params), absl::nullopt);

  if (params[0] == params[3] && params[1] == params[4] &&
      params[2] == params[5]) {
    std::cerr << "ERROR: Eye and look parameters of LookAt must be different "
                 "points"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  if (params[6] == (float_t)0.0 && params[7] == (float_t)0.0 &&
      params[8] == (float_t)0.0) {
    std::cerr << "ERROR: Zero length up paramater to LookAt" << std::endl;
    exit(EXIT_FAILURE);
  }

  POINT3 pos = PointCreate(params[0], params[1], params[2]);
  POINT3 look = PointCreate(params[3], params[4], params[5]);

  VECTOR3 direction = PointSubtract(look, pos);
  direction = VectorNormalize(direction, nullptr, nullptr);

  VECTOR3 up = VectorCreate(params[6], params[7], params[8]);
  up = VectorNormalize(up, nullptr, nullptr);

  VECTOR3 right = VectorCrossProduct(up, direction);
  if (right.x == (float_t)0.0 && right.y == (float_t)0.0 &&
      right.z == (float_t)0.0) {
    std::cerr << "ERROR: Parallel look and up vectors for LookAt" << std::endl;
    exit(EXIT_FAILURE);
  }

  right = VectorNormalize(right, NULL, NULL);
  up = VectorCrossProduct(direction, right);

  Matrix transform;
  ISTATUS status = MatrixAllocate(
      right.x, up.x, direction.x, pos.x, right.y, up.y, direction.y, pos.y,
      right.z, up.z, direction.z, pos.z, (float_t)0.0, (float_t)0.0,
      (float_t)0.0, (float_t)1.0, transform.release_and_get_address());

  switch (status) {
    case ISTATUS_ARITHMETIC_ERROR:
      std::cerr << "ERROR: Non-invertible matrix" << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_ALLOCATION_FAILED:
      ReportOOM();
    default:
      assert(status == ISTATUS_SUCCESS);
  }

  Transform(transform);
}

void MatrixManager::CoordinateSystem(Directive& directive) {
  auto name = directive.SingleQuotedString("name");
  m_coordinate_systems[name] = m_current;
}

void MatrixManager::CoordSysTransform(Directive& directive) {
  auto name = directive.SingleQuotedString("name");
  auto result = m_coordinate_systems.find(name);
  if (result != m_coordinate_systems.end()) {
    std::cerr << "WARNING: No coordinate system with name '" << name
              << "' found" << std::endl;
  } else {
    m_current = result->second;
  }
}

void MatrixManager::Transform(Directive& directive) {
  std::array<std::string, 16> unparsed_params;
  std::array<float_t, 16> params;
  directive.FiniteFloats(absl::MakeSpan(params),
                         absl::MakeSpan(unparsed_params));

  Matrix transform;
  ISTATUS status =
      MatrixAllocate(params[0], params[1], params[2], params[3], params[4],
                     params[5], params[6], params[7], params[8], params[9],
                     params[10], params[11], params[12], params[13], params[14],
                     params[15], transform.release_and_get_address());

  switch (status) {
    case ISTATUS_ARITHMETIC_ERROR:
      std::cerr << "ERROR: Transform parameters were non-invertible: ["
                << absl::StrJoin(unparsed_params, ", ") << " ]" << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_ALLOCATION_FAILED:
      ReportOOM();
    default:
      assert(status == ISTATUS_SUCCESS);
  }

  Set(transform);
}

void MatrixManager::ConcatTransform(Directive& directive) {
  std::array<std::string, 16> unparsed_params;
  std::array<float_t, 16> params;
  directive.FiniteFloats(absl::MakeSpan(params),
                         absl::MakeSpan(unparsed_params));

  Matrix transform;
  ISTATUS status =
      MatrixAllocate(params[0], params[1], params[2], params[3], params[4],
                     params[5], params[6], params[7], params[8], params[9],
                     params[10], params[11], params[12], params[13], params[14],
                     params[15], transform.release_and_get_address());

  switch (status) {
    case ISTATUS_ARITHMETIC_ERROR:
      std::cerr << "ERROR: ConcatTransform parameters were non-invertible: ["
                << absl::StrJoin(unparsed_params, ", ") << " ]" << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_ALLOCATION_FAILED:
      ReportOOM();
    default:
      assert(status == ISTATUS_SUCCESS);
  }

  Transform(transform);
}

void MatrixManager::ActiveTransform(Directive& directive) {
  auto parameter = directive.SingleString("parameter");

  if (parameter == "StartTime") {
    m_active = MatrixManager::START_TRANSFORM;
    return;
  }

  if (parameter == "EndTime") {
    m_active = MatrixManager::END_TRANSFORM;
    return;
  }

  if (parameter == "All") {
    m_active = MatrixManager::ALL_TRANSFORMS;
    return;
  }

  std::cerr << "ERROR: Failed to parse ActiveTransform parameter: " << parameter
            << std::endl;
  exit(EXIT_FAILURE);
}

void MatrixManager::Transform(Matrix m) {
  if (m_active & START_TRANSFORM) {
    Matrix old_value = m_current.first;
    ISTATUS status = MatrixAllocateProduct(
        old_value.get(), m.get(), m_current.first.release_and_get_address());

    switch (status) {
      case ISTATUS_ARITHMETIC_ERROR:
        std::cerr << "ERROR: Non-invertible matrix product" << std::endl;
        exit(EXIT_FAILURE);
      case ISTATUS_ALLOCATION_FAILED:
        ReportOOM();
      default:
        assert(status == ISTATUS_SUCCESS);
    }
  }

  if (m_active & END_TRANSFORM) {
    Matrix old_value = m_current.second;
    ISTATUS status = MatrixAllocateProduct(
        old_value.get(), m.get(), m_current.second.release_and_get_address());

    switch (status) {
      case ISTATUS_ARITHMETIC_ERROR:
        std::cerr << "ERROR: Non-invertible matrix product" << std::endl;
        exit(EXIT_FAILURE);
      case ISTATUS_ALLOCATION_FAILED:
        ReportOOM();
      default:
        assert(status == ISTATUS_SUCCESS);
    }
  }
}

void MatrixManager::Set(Matrix m) {
  if (m_active & START_TRANSFORM) {
    m_current.first = m;
  }

  if (m_active & END_TRANSFORM) {
    m_current.second = m;
  }
}

typedef std::tuple<Camera, Matrix, Sampler, Framebuffer, Integrator,
                   LightSamplerFactory, ColorExtrapolator, ColorIntegrator,
                   OutputWriter, Random, SpectralRepresentation>
    GlobalConfig;

class GlobalParser {
 public:
  static GlobalConfig Parse(Tokenizer& tokenizer,
                            MatrixManager& matrix_manager);

 private:
  GlobalParser(Tokenizer& tokenizer, MatrixManager& matrix_manager)
      : m_tokenizer(tokenizer), m_matrix_manager(matrix_manager) {}

  bool ParseDirectiveOnce(absl::string_view name, absl::string_view token,
                          void (GlobalParser::*implementation)(Directive&));
  void Accelerator(Directive& directive);
  void Camera(Directive& directive);
  void ColorExtrapolator(Directive& directive);
  void ColorIntegrator(Directive& directive);
  void Film(Directive& directive);
  void Integrator(Directive& directive);
  void PixelFilter(Directive& directive);
  void Random(Directive& directive);
  void Sampler(Directive& directive);
  void SpectralRepresentation(Directive& directive);

  void Parse();

  Tokenizer& m_tokenizer;
  MatrixManager& m_matrix_manager;

  std::set<absl::string_view> m_called;
  absl::optional<CameraFactory> m_camera_factory;
  absl::optional<iris::ColorExtrapolator> m_color_extrapolator;
  absl::optional<iris::ColorIntegrator> m_color_integrator;
  absl::optional<FilmResult> m_film_result;
  absl::optional<IntegratorResult> m_integrator_result;
  absl::optional<iris::Random> m_random;
  absl::optional<iris::Sampler> m_sampler;
  absl::optional<iris::SpectralRepresentation> m_spectral_representation;
  Matrix m_camera_to_world;
};

bool GlobalParser::ParseDirectiveOnce(
    absl::string_view name, absl::string_view token,
    void (GlobalParser::*implementation)(Directive&)) {
  if (token != name) {
    return false;
  }

  if (!m_called.insert(name).second) {
    std::cerr << "ERROR: Invalid " << name
              << " specified more than once before WorldBegin" << std::endl;
    exit(EXIT_FAILURE);
  }

  Directive directive(name, m_tokenizer);
  (this->*implementation)(directive);

  return true;
}

void GlobalParser::Accelerator(Directive& directive) { directive.Ignore(); }

void GlobalParser::Camera(Directive& directive) {
  m_camera_factory = ParseCamera(directive);
  m_camera_to_world = m_matrix_manager.GetCurrent().first;
}

void GlobalParser::ColorExtrapolator(Directive& directive) {
  m_color_extrapolator = ParseColorExtrapolator(directive);
}

void GlobalParser::ColorIntegrator(Directive& directive) {
  m_color_integrator = ParseColorIntegrator(directive);
}

void GlobalParser::Film(Directive& directive) {
  m_film_result = ParseFilm(directive);
}

void GlobalParser::Integrator(Directive& directive) {
  m_integrator_result = ParseIntegrator(directive);
}

void GlobalParser::PixelFilter(Directive& directive) { directive.Ignore(); }

void GlobalParser::Random(Directive& directive) {
  m_random = ParseRandom(directive);
}

void GlobalParser::Sampler(Directive& directive) {
  m_sampler = ParseSampler(directive);
}

void GlobalParser::SpectralRepresentation(Directive& directive) {
  m_spectral_representation = ParseSpectralRepresentation(directive);
}

void GlobalParser::Parse() {
  m_matrix_manager.Reset();
  for (auto token = m_tokenizer.Next(); token; token = m_tokenizer.Next()) {
    if (token == "WorldBegin") {
      return;
    }

    if (m_matrix_manager.Parse(*token, m_tokenizer)) {
      continue;
    }

    if (TryParseInclude(*token, m_tokenizer)) {
      continue;
    }

    if (ParseDirectiveOnce("Accelerator", *token, &GlobalParser::Accelerator)) {
      continue;
    }

    if (ParseDirectiveOnce("Camera", *token, &GlobalParser::Camera)) {
      continue;
    }

    if (ParseDirectiveOnce("ColorExtrapolator", *token,
                           &GlobalParser::ColorExtrapolator)) {
      continue;
    }

    if (ParseDirectiveOnce("ColorIntegrator", *token,
                           &GlobalParser::ColorIntegrator)) {
      continue;
    }

    if (ParseDirectiveOnce("Film", *token, &GlobalParser::Film)) {
      continue;
    }

    if (ParseDirectiveOnce("Integrator", *token, &GlobalParser::Integrator)) {
      continue;
    }

    if (ParseDirectiveOnce("PixelFilter", *token, &GlobalParser::PixelFilter)) {
      continue;
    }

    if (ParseDirectiveOnce("Random", *token, &GlobalParser::Random)) {
      continue;
    }

    if (ParseDirectiveOnce("Sampler", *token, &GlobalParser::Sampler)) {
      continue;
    }

    if (ParseDirectiveOnce("SpectralRepresentation", *token,
                           &GlobalParser::SpectralRepresentation)) {
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

    std::cerr << "ERROR: Invalid directive before WorldBegin: " << *token
              << std::endl;
    exit(EXIT_FAILURE);
  }

  std::cerr << "ERROR: Missing WorldBegin directive" << std::endl;
  exit(EXIT_FAILURE);
}

GlobalConfig GlobalParser::Parse(Tokenizer& tokenizer,
                                 MatrixManager& matrix_manager) {
  GlobalParser parser(tokenizer, matrix_manager);
  parser.Parse();

  if (!parser.m_sampler.has_value()) {
    parser.m_sampler = CreateDefaultSampler();
  }

  if (!parser.m_film_result.has_value()) {
    parser.m_film_result = CreateDefaultFilm();
  }

  if (!parser.m_integrator_result.has_value()) {
    parser.m_integrator_result = CreateDefaultIntegrator();
  }

  if (!parser.m_camera_factory.has_value()) {
    parser.m_camera_factory = CreateDefaultCamera();
  }

  if (!parser.m_color_extrapolator.has_value()) {
    parser.m_color_extrapolator = CreateDefaultColorExtrapolator();
  }

  if (!parser.m_color_integrator.has_value()) {
    parser.m_color_integrator = CreateDefaultColorIntegrator();
  }

  if (!parser.m_random.has_value()) {
    parser.m_random = CreateDefaultRandom();
  }

  if (!parser.m_spectral_representation.has_value()) {
    parser.m_spectral_representation = CreateDefaultSpectralRepresentation();
  }

  auto camera = parser.m_camera_factory.value()(parser.m_film_result->first);

  return std::make_tuple(std::move(camera), std::move(parser.m_camera_to_world),
                         std::move(parser.m_sampler.value()),
                         std::move(parser.m_film_result->first),
                         std::move(parser.m_integrator_result->first),
                         std::move(parser.m_integrator_result->second),
                         std::move(parser.m_color_extrapolator.value()),
                         std::move(parser.m_color_integrator.value()),
                         std::move(parser.m_film_result->second),
                         std::move(parser.m_random.value()),
                         std::move(parser.m_spectral_representation.value()));
}

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

  std::stack<ShaderState> m_shader_state;
  std::stack<TransformState> m_transform_state;
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
  static std::pair<Scene, std::vector<Light>> Parse(
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
  m_matrix_manager.Reset();
  for (auto token = m_tokenizer.Next(); token; token = m_tokenizer.Next()) {
    if (token == "WorldEnd") {
      return m_scene_builder.Build();
    }

    if (TryParseInclude(*token, m_tokenizer)) {
      continue;
    }

    if (m_matrix_manager.Parse(*token, m_tokenizer)) {
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

std::pair<Scene, std::vector<Light>> GeometryParser::Parse(
    Tokenizer& tokenizer, MatrixManager& matrix_manager,
    SpectrumManager& spectrum_manager) {
  GeometryParser parser(tokenizer, matrix_manager, spectrum_manager);
  return parser.Parse();
}

const COLOR_SPACE kDefaultRgbColorSpace = COLOR_SPACE_LINEAR_SRGB;

std::pair<SpectrumManager, ColorIntegrator> CreateSpectrumManager(
    ColorExtrapolator color_extrapolator, ColorIntegrator color_integrator,
    SpectralRepresentation spectral_representation,
    absl::optional<COLOR_SPACE> rgb_color_space_override) {
  COLOR_SPACE rgb_color_space =
      rgb_color_space_override.value_or(kDefaultRgbColorSpace);

  if (!spectral_representation.color_space.has_value()) {
    return std::make_pair(
        SpectrumManager(std::move(color_extrapolator), rgb_color_space),
        color_integrator);
  }

  ColorIntegrator final_color_integrator;
  ISTATUS status = ColorColorIntegratorAllocate(
      *spectral_representation.color_space,
      final_color_integrator.release_and_get_address());
  SuccessOrOOM(status);

  ColorExtrapolator extrapolator;
  status =
      ColorColorExtrapolatorAllocate(*spectral_representation.color_space,
                                     extrapolator.release_and_get_address());
  SuccessOrOOM(status);

  return std::make_pair(SpectrumManager(std::move(extrapolator),
                                        color_integrator, rgb_color_space),
                        final_color_integrator);
}

}  // namespace

Parser Parser::Create(absl::string_view file) {
  Parser result;
  result.m_tokenizer = Tokenizer::CreateFromFile(file);
  return result;
}

Parser Parser::Create(std::istream& stream) {
  Parser result;
  result.m_tokenizer = Tokenizer::CreateFromStream(stream);
  return result;
}

absl::optional<RendererConfiguration> Parser::Next(
    absl::optional<SpectralRepresentation> spectral_representation_override,
    absl::optional<COLOR_SPACE> rgb_color_space_override,
    absl::optional<bool> always_compute_reflective_color_override) {
  if (Done()) {
    return absl::nullopt;
  }

  MatrixManager matrix_manager;
  auto global_config = GlobalParser::Parse(m_tokenizer, matrix_manager);

  if (always_compute_reflective_color_override.value_or(false)) {
    ColorIntegrator old = std::get<7>(global_config);
    ISTATUS status = ReflectiveColorIntegratorAllocate(
        old.get(), std::get<7>(global_config).release_and_get_address());
    SuccessOrOOM(status);
  }

  auto manager_and_interpolator = CreateSpectrumManager(
      std::move(std::get<6>(global_config)),
      std::move(std::get<7>(global_config)),
      spectral_representation_override.value_or(std::get<10>(global_config)),
      rgb_color_space_override);

  auto geometry_config = GeometryParser::Parse(m_tokenizer, matrix_manager,
                                               manager_and_interpolator.first);

  return std::make_tuple(
      std::move(geometry_config.first),
      std::move(std::get<5>(global_config)(geometry_config.second)),
      std::move(std::get<0>(global_config)),
      std::move(std::get<1>(global_config)),
      std::move(std::get<2>(global_config)),
      std::move(std::get<4>(global_config)),
      std::move(manager_and_interpolator.second),
      std::move(std::get<9>(global_config)),
      std::move(std::get<3>(global_config)),
      std::move(std::get<8>(global_config)));
}

bool Parser::Done() { return !m_tokenizer.Peek().has_value(); }

}  // namespace iris