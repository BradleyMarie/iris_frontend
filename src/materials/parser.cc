#include "src/materials/parser.h"

#include <iostream>

#include "src/common/call_directive.h"
#include "src/materials/matte.h"

namespace iris {

Material ParseMaterial(const char* base_type_name, Tokenizer& tokenizer,
                       SpectrumManager& spectrum_manager,
                       TextureManager& texture_manager) {
  return CallDirective<Material, 1, SpectrumManager&, TextureManager&>(
      base_type_name, tokenizer, {std::make_pair("matte", ParseMatte)},
      spectrum_manager, texture_manager);
}

}  // namespace iris