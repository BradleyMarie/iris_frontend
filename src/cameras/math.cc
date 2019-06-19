#include "src/cameras/math.h"

#include <cmath>
#include <utility>

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

static const float_t kImagePlaneDistance = (float_t)1.0;

}  // namespace

CameraDimensions ComputeCameraDimensions(const Matrix& camera_to_world,
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

  CameraDimensions result;
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

}  // namespace iris