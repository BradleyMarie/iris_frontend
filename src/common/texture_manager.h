#ifndef _SRC_COMMON_TEXTURE_MANAGER_
#define _SRC_COMMON_TEXTURE_MANAGER_

#include <set>

#include "absl/container/flat_hash_map.h"
#include "absl/strings/string_view.h"
#include "src/common/pointer_types.h"

namespace iris {

class TextureManager {
 public:
  const std::pair<ReflectorTexture, std::set<Reflector>>& GetReflectorTexture(
      const absl::string_view name) const;
  void SetReflectorTexture(absl::string_view name,
                           const ReflectorTexture& texture,
                           const std::set<Reflector>& texture_reflectors);

  const FloatTexture& GetFloatTexture(const absl::string_view name) const;
  void SetFloatTexture(absl::string_view name, const FloatTexture& texture);

 private:
  absl::flat_hash_map<std::string, FloatTexture> m_float_textures;
  absl::flat_hash_map<std::string,
                      std::pair<ReflectorTexture, std::set<Reflector>>>
      m_reflector_textures;
};

}  // namespace iris

#endif  // _SRC_COMMON_TEXTURE_MANAGER_