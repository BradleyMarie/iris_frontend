#include "src/color_integrators/cie.h"

#include <iostream>

#include "iris_physx_toolkit/cie_color_integrator.h"
#include "src/common/error.h"
#include "src/param_matchers/matcher.h"

namespace iris {

ColorIntegrator ParseCie(Parameters& parameters,
                         bool spectrum_color_workaround) {
  parameters.Ignore();

  ColorIntegrator result;
  ISTATUS status = CieColorIntegratorAllocate(spectrum_color_workaround,
                                              result.release_and_get_address());
  SuccessOrOOM(status);

  return result;
}

}  // namespace iris