#include "src/cameras/perspective.h"

#include "iris_camera_toolkit/pinhole_camera.h"
#include "src/cameras/math.h"
#include "src/param_matcher.h"

#include <iostream>

namespace iris {
namespace {

static const float_t kPi = (float_t)3.1415926535897932384626433832;

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

    auto camera_params = ComputeCameraDimensions(
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

static const float_t kDefaultHalfFov = (float_t)45.0;
static const float_t kDefaultLensRadius = (float_t)0.0;
static const float_t kDefaultFocalDistance = (float_t)1e31;
static const float_t kDefaultAspectRatio =
    std::numeric_limits<float_t>::quiet_NaN();

}  // namespace

CameraFactory ParsePerspective(const char* base_type_name,
                               const char* type_name, Tokenizer& tokenizer,
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

}  // namespace iris