#ifndef _SRC_MATERIALS_MATTE_
#define _SRC_MATERIALS_MATTE_

#include "src/common/named_texture_manager.h"
#include "src/common/parameters.h"
#include "src/materials/result.h"

namespace iris {

MaterialResult ParseMatte(Parameters& parameters,
                          const NamedTextureManager& named_texture_manager,
                          NormalMapManager& normal_map_manager,
                          TextureManager& texture_manager,
                          SpectrumManager& spectrum_manager);

}  // namespace iris

#endif  // _SRC_MATERIALS_MATTE_