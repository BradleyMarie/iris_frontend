#include "src/cameras/perspective.h"

#include <iostream>

#include "iris_camera_toolkit/pinhole_camera.h"
#include "src/cameras/math.h"
#include "src/common/error.h"
#include "src/param_matchers/float_single.h"
#include "src/param_matchers/matcher.h"

namespace iris {
namespace {

static const float_t kPi = (float_t)3.1415926535897932384626433832;

CameraFactory CreatePerspectiveCameraFactory(
    const Matrix& camera_to_world, absl::optional<float_t> frame_aspect_ratio,
    float_t half_fov) {
  return [=](const Framebuffer& framebuffer) {
    float_t aspect_ratio;
    if (frame_aspect_ratio) {
      aspect_ratio = *frame_aspect_ratio;
    } else {
      size_t xdim, ydim;
      FramebufferGetSize(framebuffer.get(), &xdim, &ydim);
      aspect_ratio = (float_t)((double)xdim / (double)ydim);
    }

    auto camera_params = ComputeCameraDimensions(
        camera_to_world, aspect_ratio, half_fov * kPi / (float_t)180.0);

    Camera result;
    ISTATUS status = PinholeCameraAllocate(
        camera_params.camera_location, camera_params.camera_direction,
        camera_params.camera_up, camera_params.image_distance,
        camera_params.image_width, camera_params.image_height,
        result.release_and_get_address());
    SuccessOrOOM(status);

    return result;
  };
}

static const float_t kDefaultHalfFov = (float_t)45.0;
static const float_t kDefaultLensRadius = (float_t)0.0;
static const float_t kDefaultFocalDistance = (float_t)1e31;

}  // namespace

CameraFactory ParsePerspective(const char* base_type_name,
                               const char* type_name, Tokenizer& tokenizer,
                               MatrixManager& matrix_manager) {
  SingleFloatMatcher halffov(base_type_name, type_name, "halffov", false, false,
                             (float_t)0.0, (float_t)90.0, absl::nullopt);
  SingleFloatMatcher fov(base_type_name, type_name, "fov", false, false,
                         (float_t)0.0, (float_t)180.0, absl::nullopt);
  SingleFloatMatcher frameaspectratio(
      base_type_name, type_name, "frameaspectratio", false, false, (float_t)0.0,
      (float_t)INFINITY, absl::nullopt);

  MatchParameters<3>(base_type_name, type_name, tokenizer,
                     {&halffov, &fov, &frameaspectratio});

  if (halffov.Get() && fov.Get()) {
    std::cerr << "ERROR: Only one of halffov or fov may be specified "
              << std::endl;
    exit(EXIT_FAILURE);
  }

  float_t half_fov;
  if (halffov.Get()) {
    half_fov = *halffov.Get();
  } else if (fov.Get()) {
    half_fov = *fov.Get() / (float_t)2.0;
  } else {
    half_fov = kDefaultHalfFov;
  }

  return CreatePerspectiveCameraFactory(matrix_manager.GetCurrent().first,
                                        frameaspectratio.Get(), half_fov);
}

}  // namespace iris