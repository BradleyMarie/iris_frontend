#ifndef _SRC_MATERIALS_PLASTIC_
#define _SRC_MATERIALS_PLASTIC_

#include "src/common/material_factory.h"
#include "src/common/named_texture_manager.h"
#include "src/common/parameters.h"

namespace iris {

MaterialFactory ParsePlastic(Parameters& parameters,
                             const NamedTextureManager& named_texture_manager,
                             NormalMapManager& normal_map_manager,
                             TextureManager& texture_manager,
                             SpectrumManager& spectrum_manager);

}  // namespace iris

#endif  // _SRC_MATERIALS_PLASTIC_