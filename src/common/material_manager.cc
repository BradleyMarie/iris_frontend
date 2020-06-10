#include "src/common/material_manager.h"

#include <iostream>

#include "iris_physx_toolkit/matte_material.h"
#include "src/common/error.h"

namespace iris {

const Material& MaterialManager::AllocateMatteMaterial(
    const ReflectorTexture& kd) {
  Material& result = m_matte_materials[kd];
  if (!result.get()) {
    ISTATUS status =
        MatteMaterialAllocate(kd.get(), result.release_and_get_address());
    SuccessOrOOM(status);
  }

  return result;
}

}  // namespace iris