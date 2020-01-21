#ifndef _SRC_TEXTURES_PARSER_
#define _SRC_TEXTURES_PARSER_

#include "src/common/named_texture_manager.h"
#include "src/common/spectrum_manager.h"
#include "src/common/texture_manager.h"
#include "src/common/tokenizer.h"

namespace iris {

void ParseTexture(const char* base_type_name, Tokenizer& tokenizer,
                  NamedTextureManager& named_texture_manager,
                  TextureManager& texture_manager,
                  SpectrumManager& spectrum_manager,
                  ColorExtrapolator& color_extrapolator);

}  // namespace iris

#endif  // _SRC_TEXTURES_PARSER_