#ifndef _SRC_MATERIALS_UBER_
#define _SRC_MATERIALS_UBER_

#include "src/common/named_texture_manager.h"
#include "src/common/parameters.h"
#include "src/materials/result.h"

namespace iris {

MaterialResult ParseUber(Parameters& parameters,
                          const NamedTextureManager& named_texture_manager,
                          NormalMapManager& normal_map_manager,
                          TextureManager& texture_manager,
                          SpectrumManager& spectrum_manager);

}  // namespace iris

#endif  // _SRC_MATERIALS_UBER_