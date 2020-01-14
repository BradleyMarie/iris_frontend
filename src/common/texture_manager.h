#ifndef _SRC_COMMON_TEXTURE_MANAGER_
#define _SRC_COMMON_TEXTURE_MANAGER_

#include <map>

#include "src/common/pointer_types.h"

namespace iris {

class TextureManager {
 public:
  ReflectorTexture AllocateConstantReflectorTexture(const Reflector& reflector);
  FloatTexture AllocateConstantFloatTexture(float_t value);

 private:
  std::map<Reflector, ReflectorTexture> m_constant_reflector_textures;
  std::map<float_t, FloatTexture> m_constant_float_textures;
};

}  // namespace iris

#endif  // _SRC_COMMON_TEXTURE_MANAGER_