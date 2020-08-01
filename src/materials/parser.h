#ifndef _SRC_MATERIALS_PARSER_
#define _SRC_MATERIALS_PARSER_

#include "src/common/tokenizer.h"
#include "src/materials/result.h"

namespace iris {

MaterialResult ParseMaterial(absl::string_view base_type_name,
                             Tokenizer& tokenizer,
                             const NamedTextureManager& named_texture_manager,
                             NormalMapManager& normal_map_manager,
                             TextureManager& texture_manager,
                             SpectrumManager& spectrum_manager);

std::pair<std::string, MaterialResult> ParseMakeNamedMaterial(
    absl::string_view base_type_name, Tokenizer& tokenizer,
    const NamedTextureManager& named_texture_manager,
    NormalMapManager& normal_map_manager, TextureManager& texture_manager,
    SpectrumManager& spectrum_manager);

std::string ParseNamedMaterial(absl::string_view base_type_name,
                               Tokenizer& tokenizer);

}  // namespace iris

#endif  // _SRC_MATERIALS_PARSER_