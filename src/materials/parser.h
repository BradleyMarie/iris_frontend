#ifndef _SRC_MATERIALS_PARSER_
#define _SRC_MATERIALS_PARSER_

#include "src/common/directive.h"
#include "src/materials/result.h"

namespace iris {

MaterialResult ParseMaterial(Directive& directive,
                             const NamedTextureManager& named_texture_manager,
                             NormalMapManager& normal_map_manager,
                             TextureManager& texture_manager,
                             SpectrumManager& spectrum_manager);

std::pair<std::string, MaterialResult> ParseMakeNamedMaterial(
    Directive& directive, const NamedTextureManager& named_texture_manager,
    NormalMapManager& normal_map_manager, TextureManager& texture_manager,
    SpectrumManager& spectrum_manager);

std::string ParseNamedMaterial(Directive& directive);

}  // namespace iris

#endif  // _SRC_MATERIALS_PARSER_