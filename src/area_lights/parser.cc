#include "src/area_lights/parser.h"
#include "src/area_lights/diffuse.h"
#include "src/directive_parser.h"

#include <iostream>

namespace iris {

AreaLightResult ParseAreaLight(const char* base_type_name, Tokenizer& tokenizer) {
  return ParseDirective<AreaLightResult, 1>(
      base_type_name, tokenizer, {std::make_pair("diffuse", ParseDiffuse)});
}

}  // namespace iris