#include "src/color_integrators/parser.h"

#include "src/color_integrators/cie.h"

namespace iris {
namespace {

const Directive::Implementations<ColorIntegrator> kImpls = {
    {"cie", ParseCie}};

}  // namespace

ColorIntegrator ParseColorIntegrator(Directive& directive) {
  return directive.Invoke(kImpls);
}

ColorIntegrator CreateDefaultColorIntegrator() {
  Parameters parameters;
  return ParseCie(parameters);
}

}  // namespace iris