#include "src/light_propagation/parser.h"

#include "src/light_propagation/color.h"
#include "src/light_propagation/spectrum.h"

namespace iris {
namespace {

const Directive::Implementations<LightPropagationResult> kImpls = {
    {"color", ParseColor}, {"spectrum", ParseSpectrum}};

}  // namespace

LightPropagationResult ParseLightPropagation(Directive& directive) {
  return directive.Invoke(kImpls);
}

LightPropagationResult CreateDefaultLightPropagation(bool spectral) {
  Parameters parameters;
  if (spectral) {
    return ParseSpectrum(parameters);
  }
  return ParseColor(parameters);
}

}  // namespace iris