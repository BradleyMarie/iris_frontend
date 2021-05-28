#include "src/common/material_manager.h"

#include "iris_physx_toolkit/alpha_material.h"
#include "iris_physx_toolkit/matte_material.h"
#include "iris_physx_toolkit/mirror_material.h"
#include "iris_physx_toolkit/plastic_material.h"
#include "src/common/error.h"

namespace iris {

const Material& MaterialManager::AllocateAlphaMaterial(
    const Material& material, const FloatTexture& alpha) {
  Material& result = m_alpha_materials[std::make_pair(material, alpha)];
  if (!result.get()) {
    ISTATUS status = AlphaMaterialAllocate(material.get(), alpha.get(),
                                           result.release_and_get_address());
    SuccessOrOOM(status);
  }

  return result;
}

const Material& MaterialManager::AllocateMatteMaterial(
    const ReflectorTexture& kd, const FloatTexture& sigma) {
  Material& result = m_matte_materials[std::make_pair(kd, sigma)];
  if (!result.get()) {
    ISTATUS status = MatteMaterialAllocate(kd.get(), sigma.get(),
                                           result.release_and_get_address());
    SuccessOrOOM(status);
  }

  return result;
}

const Material& MaterialManager::AllocateMirrorMaterial(
    const ReflectorTexture& kr) {
  Material& result = m_mirror_materials[kr];
  if (!result.get()) {
    ISTATUS status =
        MirrorMaterialAllocate(kr.get(), result.release_and_get_address());
    SuccessOrOOM(status);
  }

  return result;
}

const Material& MaterialManager::AllocatePlasticMaterial(
    const ReflectorTexture& kd, const ReflectorTexture& ks,
    const FloatTexture& roughness, bool remap_roughness) {
  Material& result =
      m_plastic_materials[std::make_tuple(kd, ks, roughness, remap_roughness)];
  if (!result.get()) {
    ISTATUS status = PlasticMaterialAllocate(kd.get(), ks.get(),
                                             roughness.get(), remap_roughness,
                                             result.release_and_get_address());
    SuccessOrOOM(status);
  }

  return result;
}

}  // namespace iris