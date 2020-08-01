#include "src/color_integrators/parser.h"

#include "src/color_integrators/cie.h"

namespace iris {
namespace {

const Directive::Implementations<ColorIntegrator, bool> kImpls = {
    {"cie", ParseCie}};

}  // namespace

ColorIntegrator ParseColorIntegrator(Directive& directive,
                                     bool spectrum_color_workaround) {
  return directive.Invoke(kImpls, spectrum_color_workaround);
}

ColorIntegrator CreateDefaultColorIntegrator(bool spectrum_color_workaround) {
  Parameters parameters;
  return ParseCie(parameters, spectrum_color_workaround);
}

}  // namespace iris