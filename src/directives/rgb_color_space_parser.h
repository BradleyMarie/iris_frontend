#ifndef _SRC_DIRECTIVES_RGB_COLOR_SPACE_PARSER_
#define _SRC_DIRECTIVES_RGB_COLOR_SPACE_PARSER_

#include "src/common/directive.h"
#include "src/directives/color_space.h"

namespace iris {

COLOR_SPACE ParseRgbColorSpace(Directive& directive);
COLOR_SPACE CreateDefaultRgbColorSpace();

}  // namespace iris

#endif  // _SRC_DIRECTIVES_RGB_COLOR_SPACE_PARSER_