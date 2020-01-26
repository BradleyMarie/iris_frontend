#include "src/color_integrators/cie.h"

#include <iostream>

#include "iris_physx_toolkit/cie_color_integrator.h"
#include "src/common/error.h"
#include "src/param_matchers/matcher.h"

namespace iris {

ColorIntegrator ParseCie(const char* base_type_name, const char* type_name,
                         Tokenizer& tokenizer, bool spectrum_color_workaround) {
  MatchParameters<0>(base_type_name, type_name, tokenizer, {});

  ColorIntegrator result;
  ISTATUS status = CieColorIntegratorAllocate(spectrum_color_workaround,
                                              result.release_and_get_address());
  SuccessOrOOM(status);

  return result;
}

}  // namespace iris