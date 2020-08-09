#include "src/color_extrapolators/smits.h"

#include "iris_physx_toolkit/smits_color_extrapolator.h"
#include "src/common/error.h"

namespace iris {

ColorExtrapolator ParseSmits(Parameters& parameters) {
  parameters.Match();

  std::vector<float_t> wavelengths;
  for (size_t lambda = 360; lambda <= 830; lambda++) {
    wavelengths.push_back(lambda);
  }

  ColorExtrapolator result;
  ISTATUS status =
      SmitsColorExtrapolatorAllocate(wavelengths.data(), wavelengths.size(),
                                     result.release_and_get_address());
  SuccessOrOOM(status);

  return result;
}

}  // namespace iris