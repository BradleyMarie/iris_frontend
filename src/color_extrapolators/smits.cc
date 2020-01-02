#include "src/color_extrapolators/smits.h"

#include <iostream>

#include "iris_physx_toolkit/rgb_interpolator.h"
#include "src/common/error.h"
#include "src/param_matchers/matcher.h"

namespace iris {

ColorExtrapolator ParseSmits(const char* base_type_name, const char* type_name,
                             Tokenizer& tokenizer) {
  MatchParameters<0>(base_type_name, type_name, tokenizer, {});

  std::vector<float_t> wavelengths;
  for (size_t lambda = 360; lambda <= 830; lambda++) {
    wavelengths.push_back(lambda);
  }

  ColorExtrapolator result;
  ISTATUS status = RgbInterpolatorAllocate(
      wavelengths.data(), wavelengths.size(), result.release_and_get_address());
  SuccessOrOOM(status);

  return result;
}

}  // namespace iris