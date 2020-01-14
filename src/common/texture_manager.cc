#include "src/common/texture_manager.h"

#include <iostream>

#include "iris_physx_toolkit/constant_texture.h"
#include "src/common/error.h"

namespace iris {

const ReflectorTexture& TextureManager::GetReflectorTexture(
    const absl::string_view name) const {
  auto iter = m_reflector_textures.find(name);
  if (iter == m_reflector_textures.end()) {
    std::cerr << "ERROR: Texture not defined: " << name << std::endl;
    exit(EXIT_FAILURE);
  }
  return iter->second;
}

void TextureManager::SetReflectorTexture(absl::string_view name,
                                         const ReflectorTexture& texture) {
  m_reflector_textures[name] = texture;
}

const FloatTexture& TextureManager::GetFloatTexture(
    absl::string_view name) const {
  auto iter = m_float_textures.find(name);
  if (iter == m_float_textures.end()) {
    std::cerr << "ERROR: Texture not defined: " << name << std::endl;
    exit(EXIT_FAILURE);
  }
  return iter->second;
}

void TextureManager::SetFloatTexture(const absl::string_view name,
                                     const FloatTexture& texture) {
  m_float_textures[name] = texture;
}

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