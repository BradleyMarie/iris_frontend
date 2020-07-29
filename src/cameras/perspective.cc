#include "src/cameras/perspective.h"

#include <iostream>

#include "iris_camera_toolkit/pinhole_camera.h"
#include "src/common/error.h"
#include "src/param_matchers/float_single.h"
#include "src/param_matchers/matcher.h"

namespace iris {
namespace {

std::pair<float_t, float_t> ComputeImageDimensions(float_t image_distance,
                                                   float_t aspect_ratio,
                                                   float_t half_fov_degrees) {
  const float_t kPi = (float_t)3.1415926535897932384626433832;

  float_t half_fov_radians = half_fov_degrees * kPi / (float_t)180.0;
  float_t image_dimension = image_distance * std::tan(half_fov_radians);

  float_t xdim, ydim;
  if (aspect_ratio < (float_t)1.0) {
    xdim = image_dimension;
    ydim = image_dimension / aspect_ratio;
  } else {
    xdim = image_dimension * aspect_ratio;
    ydim = image_dimension;
  }

  return std::make_pair((float_t)2.0 * xdim, (float_t)2.0 * ydim);
}

static const float_t kImagePlaneDistance = (float_t)1.0;
static const float_t kImagePlaneWidth = (float_t)1.0;
static const float_t kImagePlaneHeight = (float_t)1.0;
static const POINT3 kCameraLocation = {(float_t)0.0, (float_t)0.0,
                                       (float_t)0.0};
static const VECTOR3 kCameraDirection = {(float_t)0.0, (float_t)0.0,
                                         (float_t)1.0};
static const VECTOR3 kCameraUp = {(float_t)0.0, (float_t)1.0, (float_t)0.0};

CameraFactory CreatePerspectiveCameraFactory(
    absl::optional<float_t> frame_aspect_ratio, float_t half_fov) {
  return [=](const Framebuffer& framebuffer) {
    float_t aspect_ratio;
    if (frame_aspect_ratio) {
      aspect_ratio = *frame_aspect_ratio;
    } else {
      size_t xdim, ydim;
      FramebufferGetSize(framebuffer.get(), &xdim, &ydim);
      aspect_ratio = (float_t)((double)xdim / (double)ydim);
    }

    auto image_dimensions =
        ComputeImageDimensions(kImagePlaneDistance, aspect_ratio, half_fov);

    Camera result;
    ISTATUS status = PinholeCameraAllocate(
        kCameraLocation, kCameraDirection, kCameraUp, kImagePlaneDistance,
        image_dimensions.first, image_dimensions.second,
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
                               const char* type_name, Tokenizer& tokenizer) {
  SingleFloatMatcher halffov(base_type_name, type_name, "halffov", false, false,
                             (float_t)0.0, (float_t)90.0, absl::nullopt);
  SingleFloatMatcher fov(base_type_name, type_name, "fov", false, false,
                         (float_t)0.0, (float_t)180.0, absl::nullopt);
  SingleFloatMatcher frameaspectratio(
      base_type_name, type_name, "frameaspectratio", false, false, (float_t)0.0,
      (float_t)INFINITY, absl::nullopt);

  MatchParameters(base_type_name, type_name, tokenizer,
                  {&halffov, &fov, &frameaspectratio});

  if (halffov.Get() && fov.Get()) {
    std::cerr << "ERROR: Only one of halffov or fov may be specified"
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

  return CreatePerspectiveCameraFactory(frameaspectratio.Get(), half_fov);
}

}  // namespace iris