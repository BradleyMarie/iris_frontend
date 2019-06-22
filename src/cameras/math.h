#ifndef _SRC_CAMERAS_MATH_
#define _SRC_CAMERAS_MATH_

#include "src/common/pointer_types.h"

namespace iris {

struct CameraDimensions {
  POINT3 camera_location;
  VECTOR3 camera_direction;
  VECTOR3 camera_up;
  float_t image_distance;
  float_t image_width;
  float_t image_height;
};

CameraDimensions ComputeCameraDimensions(const Matrix& camera_to_world,
                                         float_t aspect_ratio,
                                         float_t half_fov_radians);

}  // namespace iris

#endif  // _SRC_CAMERAS_MATH_