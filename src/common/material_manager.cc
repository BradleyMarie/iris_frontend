#include "src/common/material_manager.h"

#include <iostream>

#include "iris_physx_toolkit/matte_material.h"
#include "src/common/error.h"

namespace iris {

const Material& MaterialManager::GetMaterial(absl::string_view name) const {
  auto iter = m_materials.find(name);
  if (iter == m_materials.end()) {
    std::cerr << "ERROR: Material not defined: " << name << std::endl;
    exit(EXIT_FAILURE);
  }
  return iter->second;
}

void MaterialManager::SetMaterial(absl::string_view name,
                                  const Material& material) {
  m_materials[name] = material;
}

Material MaterialManager::AllocateMatteMaterial(const ReflectorTexture& kd) {
  auto iter = m_matte_materials.find(kd);
  if (iter != m_matte_materials.end()) {
    return iter->second;
  }

  Material result;
  ISTATUS status =
      MatteMaterialAllocate(kd.get(), result.release_and_get_address());
  SuccessOrOOM(status);

  m_matte_materials[kd] = result;

  return result;
}

}  // namespace iris