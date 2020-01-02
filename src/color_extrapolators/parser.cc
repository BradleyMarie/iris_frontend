#include "src/color_extrapolators/parser.h"

#include <iostream>

#include "src/color_extrapolators/smits.h"
#include "src/common/call_directive.h"

namespace iris {

ColorExtrapolator ParseColorExtrapolator(const char* base_type_name,
                                         Tokenizer& tokenizer) {
  return CallDirective<ColorExtrapolator, 1>(
      base_type_name, tokenizer, {std::make_pair("smits", ParseSmits)});
}

ColorExtrapolator CreateDefaultColorExtrapolator() {
  Tokenizer tokenizer;
  return ParseSmits("Unused", "Unused", tokenizer);
}

}  // namespace iris