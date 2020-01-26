#include "src/color_integrators/parser.h"

#include <iostream>

#include "src/color_integrators/cie.h"
#include "src/common/call_directive.h"

namespace iris {

ColorIntegrator ParseColorIntegrator(const char* base_type_name,
                                     Tokenizer& tokenizer,
                                     bool spectrum_color_workaround) {
  return CallDirective<ColorIntegrator, 1>(base_type_name, tokenizer,
                                           {std::make_pair("cie", ParseCie)},
                                           spectrum_color_workaround);
}

ColorIntegrator CreateDefaultColorIntegrator(bool spectrum_color_workaround) {
  Tokenizer tokenizer("");
  return ParseCie("Unused", "Unused", tokenizer, spectrum_color_workaround);
}

}  // namespace iris