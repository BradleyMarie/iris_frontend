#include "src/directives/pbrt_workaround.h"

namespace iris {
namespace {

const bool kDefaultAlwaysCreateReflectiveColor = true;

}  // namespace

bool ParseAlwaysComputeReflectiveColor(Directive& directive) {
  std::string enabled = directive.SingleQuotedString("status");

  if (enabled == "true") {
    return true;
  }

  if (enabled == "false") {
    return false;
  }

  std::cerr << "ERROR: Invalid AlwaysComputeReflectiveColor status: " << enabled
            << std::endl;
  exit(EXIT_FAILURE);
}

bool CreateDefaultAlwaysComputeReflectiveColor() {
  return kDefaultAlwaysCreateReflectiveColor;
}

}  // namespace iris