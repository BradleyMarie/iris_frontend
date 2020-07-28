#include "src/common/error.h"

#include <iostream>

namespace iris {

void ReportOOM [[noreturn]] () {
  std::cerr << "ERROR: Allocation failed" << std::endl;
  exit(EXIT_FAILURE);
}

void SuccessOrOOM(ISTATUS status) {
  if (status == ISTATUS_ALLOCATION_FAILED) {
    ReportOOM();
  }

  assert(status == ISTATUS_SUCCESS);
}

}  // namespace iris