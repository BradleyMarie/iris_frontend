#ifndef _SRC_MATERIALS_PARSER_
#define _SRC_MATERIALS_PARSER_

#include "src/common/material_factory.h"
#include "src/common/named_material_manager.h"
#include "src/common/tokenizer.h"

namespace iris {

MaterialFactory ParseMaterial(const char* base_type_name, Tokenizer& tokenizer,
                              const NamedTextureManager& named_texture_manager,
                              NormalMapManager& normal_map_manager,
                              TextureManager& texture_manager,
                              SpectrumManager& spectrum_manager);

MaterialFactory ParseMakeNamedMaterial(
    const char* base_type_name, Tokenizer& tokenizer,
    NamedMaterialManager& named_material_manager,
    const NamedTextureManager& named_texture_manager,
    NormalMapManager& normal_map_manager, TextureManager& texture_manager,
    SpectrumManager& spectrum_manager);

MaterialFactory ParseNamedMaterial(
    const char* base_type_name, Tokenizer& tokenizer,
    const NamedMaterialManager& named_material_manager);

}  // namespace iris

#endif  // _SRC_MATERIALS_PARSER_