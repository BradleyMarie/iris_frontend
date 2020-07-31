#include "src/common/normal_map_manager.h"

#include "iris_physx_toolkit/bump_map.h"
#include "src/common/error.h"

namespace iris {
namespace {

static const NormalMap kEmptyNormalMap;

}  // namespace

const NormalMap& NormalMapManager::AllocateBumpMap(
    const FloatTexture& texture) {
  if (!texture.get()) {
    return kEmptyNormalMap;
  }

  NormalMap& result = m_bump_maps[texture];
  if (!result.get()) {
    ISTATUS status =
        BumpMapAllocate(texture.get(), result.release_and_get_address());
    SuccessOrOOM(status);
  }

  return result;
}

}  // namespace iris