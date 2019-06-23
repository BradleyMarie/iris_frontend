#include "src/materials/parser.h"

#include <iostream>

#include "src/common/call_directive.h"
#include "src/materials/matte.h"

namespace iris {

MaterialResult ParseMaterial(const char* base_type_name, Tokenizer& tokenizer) {
  return CallDirective<MaterialResult, 1>(
      base_type_name, tokenizer, {std::make_pair("matte", ParseMatte)});
}

}  // namespace iris