#include "src/materials/parser.h"
#include "src/directive_parser.h"
#include "src/materials/matte.h"

#include <iostream>

namespace iris {

MaterialResult ParseMaterial(const char* base_type_name, Tokenizer& tokenizer) {
  return ParseDirective<MaterialResult, 1>(
      base_type_name, tokenizer, {std::make_pair("matte", ParseMatte)});
}

}  // namespace iris