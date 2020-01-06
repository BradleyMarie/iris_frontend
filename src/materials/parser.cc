#include "src/materials/parser.h"

#include <iostream>

#include "src/common/call_directive.h"
#include "src/materials/matte.h"

namespace iris {

MaterialResult ParseMaterial(const char* base_type_name, Tokenizer& tokenizer,
                             SpectrumManager& spectrum_manager,
                             const TextureManager& texture_manager) {
  return CallDirective<MaterialResult, 1, SpectrumManager&,
                       const TextureManager&>(
      base_type_name, tokenizer, {std::make_pair("matte", ParseMatte)},
      spectrum_manager, texture_manager);
}

}  // namespace iris