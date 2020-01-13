#include "src/common/texture_manager.h"

#include <iostream>

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

}  // namespace iris