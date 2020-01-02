#include "src/materials/parser.h"

#include <iostream>

#include "src/common/call_directive.h"
#include "src/materials/matte.h"

namespace iris {

MaterialResult ParseMaterial(const char* base_type_name, Tokenizer& tokenizer,
                             const ColorExtrapolator& color_extrapolator,
                             const TextureManager& texture_manager) {
  return CallDirective<MaterialResult, 1, const ColorExtrapolator&,
                       const TextureManager&>(
      base_type_name, tokenizer, {std::make_pair("matte", ParseMatte)},
      color_extrapolator, texture_manager);
}

}  // namespace iris