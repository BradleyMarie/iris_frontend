#include "src/color_integrators/parser.h"

#include "src/color_integrators/cie.h"
#include "src/common/call_directive.h"

namespace iris {

ColorIntegrator ParseColorIntegrator(const char* base_type_name,
                                     Tokenizer& tokenizer,
                                     bool spectrum_color_workaround) {
  return CallDirective<ColorIntegrator>(base_type_name, tokenizer,
                                        {{"cie", ParseCie}},
                                        spectrum_color_workaround);
}

ColorIntegrator CreateDefaultColorIntegrator(bool spectrum_color_workaround) {
  Parameters parameters;
  return ParseCie(parameters, spectrum_color_workaround);
}

}  // namespace iris