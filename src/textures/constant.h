#ifndef _SRC_TEXTURES_CONSTANT_
#define _SRC_TEXTURES_CONSTANT_

#include "src/common/named_texture_manager.h"
#include "src/common/spectrum_manager.h"
#include "src/common/texture_manager.h"
#include "src/common/tokenizer.h"

namespace iris {

ReflectorTexture ParseConstantReflector(
    const char* base_type_name, const char* type_name, Tokenizer& tokenizer,
    const NamedTextureManager& named_texture_manager,
    TextureManager& texture_manager, SpectrumManager& spectrum_manager,
    ColorExtrapolator& color_extrapolator);

FloatTexture ParseConstantFloat(
    const char* base_type_name, const char* type_name, Tokenizer& tokenizer,
    const NamedTextureManager& named_texture_manager,
    TextureManager& texture_manager);

}  // namespace iris

#endif  // _SRC_TEXTURES_CONSTANT_