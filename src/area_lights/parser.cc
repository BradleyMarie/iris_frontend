#include "src/area_lights/parser.h"

#include <iostream>

#include "src/area_lights/diffuse.h"
#include "src/common/call_directive.h"

namespace iris {

AreaLightResult ParseAreaLight(const char* base_type_name, Tokenizer& tokenizer,
                               const ColorExtrapolator& color_extrapolator) {
  return CallDirective<AreaLightResult, 1, const ColorExtrapolator&>(
      base_type_name, tokenizer, {std::make_pair("diffuse", ParseDiffuse)},
      color_extrapolator);
}

}  // namespace iris