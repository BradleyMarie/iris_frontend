#ifndef _SRC_DIRECTIVES_COLOR_SPACE_
#define _SRC_DIRECTIVES_COLOR_SPACE_

#include "absl/strings/string_view.h"
#include "iris_advanced/iris_advanced.h"

namespace iris {

bool ParseColorSpace(absl::string_view text, COLOR_SPACE* color_space);
std::string ColorSpaceToString(const COLOR_SPACE& color_space);

}  // namespace iris

#endif  // _SRC_DIRECTIVES_COLOR_SPACE_