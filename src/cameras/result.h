#ifndef _SRC_CAMERAS_RESULT_
#define _SRC_CAMERAS_RESULT_

#include <functional>

#include "src/common/pointer_types.h"

namespace iris {

typedef std::function<Camera(const Framebuffer& framebuffer)> CameraFactory;

}  // namespace iris

#endif  // _SRC_CAMERAS_RESULT_