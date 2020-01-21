#ifndef _SRC_TEXTURES_SCALE_
#define _SRC_TEXTURES_SCALE_

#include "src/common/named_texture_manager.h"
#include "src/common/spectrum_manager.h"
#include "src/common/texture_manager.h"
#include "src/common/tokenizer.h"

namespace iris {

ReflectorTexture ParseScaleReflector(
    const char* base_type_name, const char* type_name, Tokenizer& tokenizer,
    const NamedTextureManager& named_texture_manager,
    TextureManager& texture_manager, SpectrumManager& spectrum_manager);

FloatTexture ParseScaleFloat(const char* base_type_name, const char* type_name,
                             Tokenizer& tokenizer,
                             const NamedTextureManager& named_texture_manager,
                             TextureManager& texture_manager);

}  // namespace iris

#endif  // _SRC_TEXTURES_SCALE_