#include "src/light_propagation/parser.h"

#include "src/common/call_directive.h"
#include "src/light_propagation/color.h"
#include "src/light_propagation/spectrum.h"

namespace iris {

LightPropagationResult ParseLightPropagation(absl::string_view base_type_name,
                                             Tokenizer& tokenizer) {
  return CallDirective<LightPropagationResult>(
      base_type_name, tokenizer,
      {{"color", ParseColor}, {"spectrum", ParseSpectrum}});
}

LightPropagationResult CreateDefaultLightPropagation(bool spectral) {
  Parameters parameters;
  if (spectral) {
    return ParseSpectrum(parameters);
  }
  return ParseColor(parameters);
}

}  // namespace iris