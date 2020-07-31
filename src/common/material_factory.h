#ifndef _SRC_COMMON_MATERIAL_FACTORY_
#define _SRC_COMMON_MATERIAL_FACTORY_

#include <functional>

#include "src/common/material_manager.h"
#include "src/common/named_texture_manager.h"
#include "src/common/normal_map_manager.h"
#include "src/common/parameters.h"
#include "src/common/spectrum_manager.h"
#include "src/common/texture_manager.h"

namespace iris {

typedef std::function<std::pair<Material, NormalMap>(
    Parameters& parameters, MaterialManager& material_manager,
    const NamedTextureManager& named_texture_manager, NormalMapManager&,
    TextureManager& texture_manager, SpectrumManager& spectrum_manager)>
    MaterialFactory;

}  // namespace iris

#endif  // _SRC_COMMON_MATERIAL_FACTORY_