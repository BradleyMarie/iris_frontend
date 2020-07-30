#include "src/light_propagation/parser.h"

#include "src/common/call_directive.h"
#include "src/light_propagation/color.h"
#include "src/light_propagation/spectrum.h"

namespace iris {

LightPropagationResult ParseLightPropagation(const char* base_type_name,
                                             Tokenizer& tokenizer) {
  return CallDirective<LightPropagationResult, 2>(
      base_type_name, tokenizer,
      {std::make_pair("color", ParseColor),
       std::make_pair("spectrum", ParseSpectrum)});
}

LightPropagationResult CreateDefaultLightPropagation(bool spectral) {
  Tokenizer tokenizer;
  if (spectral) {
    return ParseSpectrum("Unused", "Unused", tokenizer);
  }
  return ParseColor("Unused", "Unused", tokenizer);
}

}  // namespace iris