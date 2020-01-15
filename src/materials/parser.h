#ifndef _SRC_MATERIALS_PARSER_
#define _SRC_MATERIALS_PARSER_

#include "src/common/material_manager.h"
#include "src/common/named_material_manager.h"
#include "src/common/named_texture_manager.h"
#include "src/common/spectrum_manager.h"
#include "src/common/texture_manager.h"
#include "src/common/tokenizer.h"

namespace iris {

Material ParseMaterial(const char* base_type_name, Tokenizer& tokenizer,
                       MaterialManager& material_manager,
                       const NamedTextureManager& named_texture_manager,
                       TextureManager& texture_manager,
                       SpectrumManager& spectrum_manager);

Material ParseMakeNamedMaterial(
    const char* base_type_name, Tokenizer& tokenizer,
    NamedMaterialManager& named_material_manager,
    MaterialManager& material_manager,
    const NamedTextureManager& named_texture_manager,
    TextureManager& texture_manager, SpectrumManager& spectrum_manager);

Material ParseNamedMaterial(const char* base_type_name, Tokenizer& tokenizer,
                            const NamedMaterialManager& named_material_manager);

}  // namespace iris

#endif  // _SRC_MATERIALS_PARSER_