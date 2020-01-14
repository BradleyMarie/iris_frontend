#include "src/materials/parser.h"

#include <iostream>

#include "src/common/call_directive.h"
#include "src/materials/matte.h"

namespace iris {

Material ParseMaterial(const char* base_type_name, Tokenizer& tokenizer,
                       MaterialManager& material_manager,
                       const NamedTextureManager& named_texture_manager,
                       TextureManager& texture_manager,
                       SpectrumManager& spectrum_manager) {
  return CallDirective<Material, 1, MaterialManager&,
                       const NamedTextureManager&, TextureManager&,
                       SpectrumManager&>(
      base_type_name, tokenizer, {std::make_pair("matte", ParseMatte)},
      material_manager, named_texture_manager, texture_manager,
      spectrum_manager);
}

}  // namespace iris