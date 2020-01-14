#include "src/common/texture_manager.h"

#include <iostream>

#include "iris_physx_toolkit/constant_texture.h"
#include "src/common/error.h"

namespace iris {

ReflectorTexture TextureManager::AllocateConstantReflectorTexture(
    const Reflector& reflector) {
  auto iter = m_constant_reflector_textures.find(reflector);
  if (iter != m_constant_reflector_textures.end()) {
    return iter->second;
  }

  ReflectorTexture result;
  ISTATUS status = ConstantReflectorTextureAllocate(
      reflector.get(), result.release_and_get_address());
  SuccessOrOOM(status);

  m_constant_reflector_textures[reflector] = result;

  return result;
}

FloatTexture TextureManager::AllocateConstantFloatTexture(float_t value) {
  auto iter = m_constant_float_textures.find(value);
  if (iter != m_constant_float_textures.end()) {
    return iter->second;
  }

  FloatTexture result;
  ISTATUS status =
      ConstantFloatTextureAllocate(value, result.release_and_get_address());
  SuccessOrOOM(status);

  m_constant_float_textures[value] = result;

  return result;
}

}  // namespace iris