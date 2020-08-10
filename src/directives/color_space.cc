#include "src/directives/color_space.h"

namespace iris {
namespace {

const char* kLinearSrgbType = "linear_srgb";
const char* kXyzType = "xyz";

}  // namespace

bool ParseColorSpace(absl::string_view text, COLOR_SPACE* color_space) {
  if (text == kLinearSrgbType) {
    *color_space = COLOR_SPACE_LINEAR_SRGB;
    return true;
  }

  if (text == kXyzType) {
    *color_space = COLOR_SPACE_XYZ;
    return true;
  }

  return false;
}

std::string ColorSpaceToString(const COLOR_SPACE& color_space) {
  switch (color_space) {
    default:
      assert(false);
    case COLOR_SPACE_LINEAR_SRGB:
      return kLinearSrgbType;
    case COLOR_SPACE_XYZ:
      return kXyzType;
  }
}

}  // namespace iris