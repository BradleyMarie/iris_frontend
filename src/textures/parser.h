#ifndef _SRC_TEXTURES_PARSER_
#define _SRC_TEXTURES_PARSER_

#include "src/common/directive.h"
#include "src/common/named_texture_manager.h"
#include "src/common/spectrum_manager.h"
#include "src/common/texture_manager.h"

namespace iris {

void ParseTexture(Directive& directive, const Matrix& texture_to_world,
                  NamedTextureManager& named_texture_manager,
                  TextureManager& texture_manager,
                  SpectrumManager& spectrum_manager);

}  // namespace iris

#endif  // _SRC_TEXTURES_PARSER_