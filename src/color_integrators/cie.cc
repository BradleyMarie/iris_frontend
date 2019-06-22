#include "src/color_integrators/cie.h"

#include "iris_physx_toolkit/cie_color_integrator.h"
#include "src/param_matchers/matcher.h"

#include <iostream>

namespace iris {

ColorIntegrator ParseCie(const char* base_type_name, const char* type_name,
                         Tokenizer& tokenizer) {
  MatchParameters<0>(base_type_name, type_name, tokenizer, {});

  ColorIntegrator result;
  ISTATUS status = CieColorIntegratorAllocate(result.release_and_get_address());
  switch (status) {
    case ISTATUS_ALLOCATION_FAILED:
      std::cerr << "ERROR: Allocation failed" << std::endl;
      exit(EXIT_FAILURE);
    default:
      assert(status == ISTATUS_SUCCESS);
  }

  return result;
}

}  // namespace iris