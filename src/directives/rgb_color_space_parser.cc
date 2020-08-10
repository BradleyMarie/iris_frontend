#include "src/directives/rgb_color_space_parser.h"

#include <iostream>

namespace iris {
namespace {

const COLOR_SPACE kDefaultRgbColorSpace = COLOR_SPACE_LINEAR_SRGB;

}  // namespace

COLOR_SPACE ParseRgbColorSpace(Directive& directive) {
  std::string representation = directive.SingleQuotedString("type");

  COLOR_SPACE result;
  bool success = ParseColorSpace(representation, &result);
  if (!success) {
    std::cerr << "ERROR: Invalid RgbColorSpace specified: "
              << representation << std::endl;
  }

  return result;
}

COLOR_SPACE CreateDefaultRgbColorSpace() {
  return kDefaultRgbColorSpace;
}

}  // namespace iris