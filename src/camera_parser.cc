#include <iostream>

#include "absl/flags/flag.h"
#include "iris_camera_toolkit/grid_pixel_sampler.h"
#include "iris_camera_toolkit/pinhole_camera.h"
#include "iris_physx_toolkit/cie_color_integrator.h"
#include "iris_physx_toolkit/one_light_sampler.h"
#include "src/camera_parser.h"
#include "src/directive_parser.h"
#include "src/integrators/parser.h"
#include "src/matrix_parser.h"
#include "src/param_matcher.h"

ABSL_FLAG(std::string, default_output, "iris.pfm",
          "The default output location if none is specified.");

namespace iris {
namespace {

std::pair<float_t, float_t> ComputeImageDimensions(float_t image_distance,
                                                   float_t aspect_ratio,
                                                   float_t half_fov_radians) {
  float_t image_dimension = image_distance * std::tan(half_fov_radians);

  float_t xdim, ydim;
  if (aspect_ratio < (float_t)1.0) {
    xdim = image_dimension;
    ydim = image_dimension / aspect_ratio;
  } else {
    xdim = image_dimension * aspect_ratio;
    ydim = image_dimension;
  }

  return std::make_pair(xdim, ydim);
}

struct CameraParameters {
  POINT3 camera_location;
  VECTOR3 camera_direction;
  VECTOR3 camera_up;
  float_t image_distance;
  float_t image_width;
  float_t image_height;
};

static const float_t kImagePlaneDistance = (float_t)1.0;

CameraParameters ComputeCameraParameters(const Matrix& camera_to_world,
                                         float_t aspect_ratio,
                                         float_t half_fov_radians) {
  auto image_dimensions = ComputeImageDimensions(
      kImagePlaneDistance, aspect_ratio, half_fov_radians);

  POINT3 camera_center = PointCreate((float_t)0.0, (float_t)0.0, (float_t)0.0);
  POINT3 camera_up = PointCreate((float_t)0.0, (float_t)1.0, (float_t)0.0);
  POINT3 image_center =
      PointCreate((float_t)0.0, (float_t)0.0, -kImagePlaneDistance);
  POINT3 image_bottom_left =
      PointCreate((float_t)-image_dimensions.first,
                  (float_t)-image_dimensions.second, -kImagePlaneDistance);
  POINT3 image_bottom_right =
      PointCreate((float_t)image_dimensions.first,
                  (float_t)-image_dimensions.second, -kImagePlaneDistance);
  POINT3 image_top_left =
      PointCreate((float_t)-image_dimensions.first,
                  (float_t)image_dimensions.second, -kImagePlaneDistance);

  camera_center = PointMatrixMultiply(camera_to_world.get(), camera_center);
  camera_up = PointMatrixMultiply(camera_to_world.get(), camera_up);
  image_center = PointMatrixMultiply(camera_to_world.get(), image_center);
  image_bottom_left =
      PointMatrixMultiply(camera_to_world.get(), image_bottom_left);
  image_bottom_right =
      PointMatrixMultiply(camera_to_world.get(), image_bottom_right);
  image_top_left = PointMatrixMultiply(camera_to_world.get(), image_top_left);

  CameraParameters result;
  result.camera_location = camera_center;
  result.camera_direction = PointSubtract(camera_center, image_center);
  result.camera_up = PointSubtract(camera_up, camera_center);
  result.image_distance =
      VectorLength(PointSubtract(camera_center, image_center));
  result.image_width =
      VectorLength(PointSubtract(image_bottom_right, image_bottom_left));
  result.image_height =
      VectorLength(PointSubtract(image_top_left, image_bottom_left));

  return result;
}

typedef std::function<Camera(const Framebuffer& framebuffer)> CameraFactory;

static const float_t kPi = (float_t)3.1415926535897932384626433832;
static const float_t kDefaultHalfFov = (float_t)45.0;
static const float_t kDefaultLensRadius = (float_t)0.0;
static const float_t kDefaultFocalDistance = (float_t)1e31;
static const float_t kDefaultAspectRatio =
    std::numeric_limits<float_t>::quiet_NaN();

CameraFactory CreatePerspectiveCameraFactory(const Matrix& camera_to_world,
                                             float_t frame_aspect_ratio,
                                             float_t half_fov) {
  return [=](const Framebuffer& framebuffer) {
    float_t aspect_ratio;
    if (std::isnan(frame_aspect_ratio)) {
      size_t xdim, ydim;
      FramebufferGetSize(framebuffer.get(), &xdim, &ydim);
      aspect_ratio = (float_t)((double)xdim / (double)ydim);
    } else {
      aspect_ratio = frame_aspect_ratio;
    }

    auto camera_params = ComputeCameraParameters(
        camera_to_world, aspect_ratio, half_fov * kPi / (float_t)180.0);

    Camera result;
    ISTATUS status = PinholeCameraAllocate(
        camera_params.camera_location, camera_params.camera_direction,
        camera_params.camera_up, camera_params.image_distance,
        camera_params.image_width, camera_params.image_height,
        result.release_and_get_address());

    switch (status) {
      case ISTATUS_ALLOCATION_FAILED:
        std::cerr << "ERROR: Allocation failed" << std::endl;
        exit(EXIT_FAILURE);
      default:
        assert(status == ISTATUS_SUCCESS);
    }

    return result;
  };
}

CameraFactory ParsePerspectiveCamera(const char* base_type_name,
                                     const char* type_name,
                                     Tokenizer& tokenizer,
                                     MatrixManager& matrix_manager) {
  PositiveBoundedSingleFloatTMatcher halffov(
      base_type_name, type_name, "halffov", (float_t)90.0,
      std::numeric_limits<float_t>::quiet_NaN());
  PositiveBoundedSingleFloatTMatcher fov(
      base_type_name, type_name, "fov", (float_t)180.0,
      std::numeric_limits<float_t>::quiet_NaN());
  PositiveFiniteSingleFloatTMatcher lensradius(
      base_type_name, type_name, "lensradius", kDefaultLensRadius);
  PositiveFiniteSingleFloatTMatcher focaldistance(
      base_type_name, type_name, "focaldistance", kDefaultFocalDistance);
  PositiveFiniteSingleFloatTMatcher frameaspectratio(
      base_type_name, type_name, "frameaspectratio", kDefaultAspectRatio);
  ParseAllParameter<5>(
      base_type_name, type_name, tokenizer,
      {&halffov, &fov, &lensradius, &focaldistance, &frameaspectratio});

  if (!std::isnan(halffov.Get()) && !std::isnan(fov.Get())) {
    std::cerr << "ERROR: Only one of halffov or fov may be specified "
              << std::endl;
    exit(EXIT_FAILURE);
  }

  float_t half_fov;
  if (!std::isnan(halffov.Get())) {
    half_fov = halffov.Get();
  } else if (!std::isnan(fov.Get())) {
    half_fov = fov.Get() / (float_t)2.0;
  } else {
    half_fov = kDefaultHalfFov;
  }

  if (lensradius.Get() != (float_t)0.0) {
    std::cerr << "ERROR: Non-zero values for lensradius currently unsupported "
              << std::endl;
    exit(EXIT_FAILURE);
  }

  Matrix camera_to_world;
  *camera_to_world.release_and_get_address() =
      MatrixGetInverse(matrix_manager.GetCurrent().first.get());

  return CreatePerspectiveCameraFactory(camera_to_world, frameaspectratio.Get(),
                                        half_fov);
}

static const bool kStratifiedSamplerDefaultJitter = false;
static const uint16_t kStratifiedSamplerDefaultXSamples = 2;
static const uint16_t kStratifiedSamplerDefaultYSamples = 2;

PixelSampler ParseStratifiedSampler(const char* base_type_name,
                                    const char* type_name, Tokenizer& tokenizer,
                                    MatrixManager& matrix_manager) {
  SingleBoolMatcher jitter(base_type_name, type_name, "jitter",
                           kStratifiedSamplerDefaultJitter);
  NonZeroSingleUInt16Matcher xsamples(base_type_name, type_name, "xsamples",
                                      kStratifiedSamplerDefaultXSamples);
  NonZeroSingleUInt16Matcher ysamples(base_type_name, type_name, "ysamples",
                                      kStratifiedSamplerDefaultXSamples);
  ParseAllParameter<3>(base_type_name, type_name, tokenizer,
                       {&jitter, &xsamples, &ysamples});

  PixelSampler result;
  ISTATUS status =
      GridPixelSamplerAllocate(xsamples.Get(), ysamples.Get(), jitter.Get(), 1,
                               1, false, result.release_and_get_address());
  switch (status) {
    case ISTATUS_ALLOCATION_FAILED:
      std::cerr << "ERROR: Allocation failed" << std::endl;
      exit(EXIT_FAILURE);
    default:
      assert(status == ISTATUS_SUCCESS);
  }

  return result;
}

static const size_t kImageFilmDefaultXResolution = 640;
static const size_t kImageFilmDefaultYResolution = 480;

std::pair<Framebuffer, OutputWriter> ParseImageFilm(
    const char* base_type_name, const char* type_name, Tokenizer& tokenizer,
    MatrixManager& matrix_manager) {
  SingleStringMatcher filename(base_type_name, type_name, "filename",
                               absl::GetFlag(FLAGS_default_output));
  NonZeroSingleSizeTMatcher xresolution(
      base_type_name, type_name, "xresolution", kImageFilmDefaultXResolution);
  NonZeroSingleSizeTMatcher yresolution(
      base_type_name, type_name, "yresolution", kImageFilmDefaultYResolution);
  ParseAllParameter<3>(base_type_name, type_name, tokenizer,
                       {&filename, &xresolution, &yresolution});

  Framebuffer framebuffer;
  ISTATUS status = FramebufferAllocate(xresolution.Get(), yresolution.Get(),
                                       framebuffer.release_and_get_address());
  switch (status) {
    case ISTATUS_ALLOCATION_FAILED:
      std::cerr << "ERROR: Allocation failed" << std::endl;
      exit(EXIT_FAILURE);
    default:
      assert(status == ISTATUS_SUCCESS);
  }

  return std::make_pair(std::move(framebuffer),
                        ParseOutputWriter(filename.Get()));
}

void PopulateUninitialzedParameters(const Matrix& camera_to_world,
                                    CameraFactory camera_factory,
                                    CameraConfig& result) {
  if (!std::get<1>(result).get()) {
    ISTATUS status = GridPixelSamplerAllocate(
        kStratifiedSamplerDefaultXSamples, kStratifiedSamplerDefaultYSamples,
        kStratifiedSamplerDefaultJitter, 1, 1, false,
        std::get<1>(result).release_and_get_address());
    switch (status) {
      case ISTATUS_ALLOCATION_FAILED:
        std::cerr << "ERROR: Allocation failed" << std::endl;
        exit(EXIT_FAILURE);
      default:
        assert(status == ISTATUS_SUCCESS);
    }
  }

  if (!std::get<2>(result).get()) {
    ISTATUS status = FramebufferAllocate(
        kImageFilmDefaultXResolution, kImageFilmDefaultYResolution,
        std::get<2>(result).release_and_get_address());
    switch (status) {
      case ISTATUS_ALLOCATION_FAILED:
        std::cerr << "ERROR: Allocation failed" << std::endl;
        exit(EXIT_FAILURE);
      default:
        assert(status == ISTATUS_SUCCESS);
    }
  }

  if (!camera_factory) {
    camera_factory = CreatePerspectiveCameraFactory(
        camera_to_world, kDefaultAspectRatio, kDefaultHalfFov);
  }

  auto default_integrator = CreateDefaultIntegrator();
  if (!std::get<3>(result).get()) {
    std::get<3>(result) = std::move(default_integrator.first);
  }

  if (!std::get<4>(result)) {
    std::get<4>(result) = default_integrator.second;
  }

  if (!std::get<6>(result)) {
    std::get<6>(result) =
        ParseOutputWriter(absl::GetFlag(FLAGS_default_output));
  }

  std::get<0>(result) = camera_factory(std::get<2>(result));

  ISTATUS status =
      CieColorIntegratorAllocate(std::get<5>(result).release_and_get_address());
  switch (status) {
    case ISTATUS_ALLOCATION_FAILED:
      std::cerr << "ERROR: Allocation failed" << std::endl;
      exit(EXIT_FAILURE);
    default:
      assert(status == ISTATUS_SUCCESS);
  }
}

template <typename Result, size_t NumImplementations>
Result ParseDirectiveOnce(
    const char* base_type_name, Tokenizer& tokenizer,
    MatrixManager& matrix_manager,
    const std::array<
        std::pair<const char*, DirectiveCallback<Result, MatrixManager&>>,
        NumImplementations>& callbacks,
    bool already_set) {
  if (already_set) {
    std::cerr << "ERROR: Invalid " << base_type_name
              << " specified more than once before WorldBegin" << std::endl;
    exit(EXIT_FAILURE);
  }

  return ParseDirective<Result, NumImplementations, MatrixManager&>(
      base_type_name, tokenizer, callbacks, matrix_manager);
}

void ErrorIfPresent(const char* base_type_name, bool already_set) {
  if (already_set) {
    std::cerr << "ERROR: Invalid " << base_type_name
              << " specified more than once before WorldBegin" << std::endl;
    exit(EXIT_FAILURE);
  }
}

}  // namespace

CameraConfig ParseCamera(Tokenizer& tokenizer, MatrixManager& matrix_manager) {
  matrix_manager.ActiveTransform(MatrixManager::ALL_TRANSFORMS);
  matrix_manager.Identity();

  CameraFactory camera_factory;
  CameraConfig result;
  for (auto token = tokenizer.Next(); token; token = tokenizer.Next()) {
    if (token == "WorldBegin") {
      PopulateUninitialzedParameters(matrix_manager.GetCurrent().first,
                                     camera_factory, result);
      return result;
    }

    if (TryParseMatrix(*token, tokenizer, matrix_manager)) {
      continue;
    }

    if (token == "Camera") {
      camera_factory = ParseDirectiveOnce<CameraFactory, 1>(
          "Camera", tokenizer, matrix_manager,
          {std::make_pair("perspective", ParsePerspectiveCamera)},
          !!camera_factory);
      continue;
    }

    if (token == "Sampler") {
      auto sampler = ParseDirectiveOnce<PixelSampler, 1>(
          "Sampler", tokenizer, matrix_manager,
          {std::make_pair("stratified", ParseStratifiedSampler)},
          std::get<1>(result).get());
      std::get<1>(result) = std::move(sampler);
      continue;
    }

    if (token == "Film") {
      auto film = ParseDirectiveOnce<std::pair<Framebuffer, OutputWriter>, 1>(
          "Film", tokenizer, matrix_manager,
          {std::make_pair("image", ParseImageFilm)}, std::get<2>(result).get());
      std::get<2>(result) = std::move(film.first);
      std::get<6>(result) = std::move(film.second);
      continue;
    }

    if (token == "Filter") {
      // TODO
      continue;
    }

    if (token == "Integrator") {
      ErrorIfPresent("Integrator", std::get<3>(result).get());
      auto integrator = ParseIntegrator("Integrator", tokenizer);
      std::get<3>(result) = std::move(integrator.first);
      std::get<4>(result) = std::move(integrator.second);
      continue;
    }

    if (token == "Accelerator") {
      // Ignored
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

}  // namespace iris