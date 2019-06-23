#include "src/area_lights/parser.h"

#include <iostream>

#include "src/area_lights/diffuse.h"
#include "src/common/call_directive.h"

namespace iris {

AreaLightResult ParseAreaLight(const char* base_type_name,
                               Tokenizer& tokenizer) {
  return CallDirective<AreaLightResult, 1>(
      base_type_name, tokenizer, {std::make_pair("diffuse", ParseDiffuse)});
}

}  // namespace iris