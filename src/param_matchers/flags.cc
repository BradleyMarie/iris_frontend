#include "src/param_matchers/flags.h"

#include <iostream>

ABSL_FLAG(COLOR_SPACE, rgb_color_space, COLOR_SPACE_LINEAR_SRGB,
          "Sets the color space used to interpret rgb parameters.");

bool AbslParseFlag(absl::string_view text, COLOR_SPACE* color_space,
                   std::string* error) {
  if (text == "xyz") {
    *color_space = COLOR_SPACE_XYZ;
    return true;
  }
  if (text == "linearsrgb") {
    *color_space = COLOR_SPACE_LINEAR_SRGB;
    return true;
  }
  std::cerr << "ERROR: rgb_color_space must be one of: xyz, linearsrgb"
            << std::endl;
  exit(EXIT_FAILURE);
}

std::string AbslUnparseFlag(COLOR_SPACE color_space) {
  if (color_space == COLOR_SPACE_XYZ) {
    return "xyz";
  }
  if (color_space == COLOR_SPACE_LINEAR_SRGB) {
    return "linearsrgb";
  }
  std::cerr << "ERROR: rgb_color_space must be one of: xyz, linearsrgb"
            << std::endl;
  exit(EXIT_FAILURE);
}