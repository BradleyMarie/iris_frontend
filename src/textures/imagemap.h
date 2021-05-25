#ifndef _SRC_TEXTURES_IMAGEMAP_
#define _SRC_TEXTURES_IMAGEMAP_

#include "src/common/named_texture_manager.h"
#include "src/common/parameters.h"
#include "src/common/spectrum_manager.h"
#include "src/common/texture_manager.h"

namespace iris {

ReflectorTexture ParseImageMapReflector(
    Parameters& parameters, const Matrix& texture_to_world,
    const NamedTextureManager& named_texture_manager,
    TextureManager& texture_manager, SpectrumManager& spectrum_manager);

FloatTexture ParseImageMapFloat(
    Parameters& parameters, const Matrix& texture_to_world,
    const NamedTextureManager& named_texture_manager,
    TextureManager& texture_manager);

}  // namespace iris

#endif  // _SRC_TEXTURES_IMAGEMAP_