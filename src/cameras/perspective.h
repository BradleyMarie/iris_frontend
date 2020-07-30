#ifndef _SRC_CAMERAS_PERSPECTIVE_
#define _SRC_CAMERAS_PERSPECTIVE_

#include "src/cameras/result.h"
#include "src/common/parameters.h"

namespace iris {

CameraFactory ParsePerspective(Parameters& parameters);

}  // namespace iris

#endif  // _SRC_CAMERAS_PERSPECTIVE_