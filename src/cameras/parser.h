#ifndef _SRC_CAMERAS_PARSER_
#define _SRC_CAMERAS_PARSER_

#include "src/cameras/result.h"
#include "src/common/directive.h"

namespace iris {

CameraFactory ParseCamera(Directive& directive);
CameraFactory CreateDefaultCamera();

}  // namespace iris

#endif  // _SRC_CAMERAS_PARSER_