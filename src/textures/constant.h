#ifndef _SRC_TEXTURES_CONSTANT_
#define _SRC_TEXTURES_CONSTANT_

#include "src/common/named_texture_manager.h"
#include "src/common/spectrum_manager.h"
#include "src/common/texture_manager.h"
#include "src/common/parameters.h"

namespace iris {

ReflectorTexture ParseConstantReflector(
    Parameters& parameters, const NamedTextureManager& named_texture_manager,
    TextureManager& texture_manager, SpectrumManager& spectrum_manager);

FloatTexture ParseConstantFloat(
    Parameters& parameters, const NamedTextureManager& named_texture_manager,
    TextureManager& texture_manager);

}  // namespace iris

#endif  // _SRC_TEXTURES_CONSTANT_