#include "src/color_integrators/cie.h"

#include "iris_physx_toolkit/cie_color_integrator.h"
#include "src/common/error.h"

namespace iris {

ColorIntegrator ParseCie(Parameters& parameters) {
  parameters.Match();

  ColorIntegrator result;
  ISTATUS status = CieColorIntegratorAllocate(result.release_and_get_address());
  SuccessOrOOM(status);

  return result;
}

}  // namespace iris