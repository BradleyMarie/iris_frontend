#ifndef _SRC_COMMON_TEXTURE_MANAGER_
#define _SRC_COMMON_TEXTURE_MANAGER_

#include <map>

#include "src/common/pointer_types.h"

namespace iris {

class TextureManager {
 public:
  ReflectorTexture AllocateConstantReflectorTexture(const Reflector& reflector);
  FloatTexture AllocateConstantFloatTexture(float_t value);

  ReflectorTexture AllocateProductReflectorTexture(
      const ReflectorTexture& tex1, const ReflectorTexture& tex2);
  FloatTexture AllocateProductFloatTexture(const FloatTexture& tex1,
                                           const FloatTexture& tex2);

 private:
  std::map<Reflector, ReflectorTexture> m_constant_reflector_textures;
  std::map<float_t, FloatTexture> m_constant_float_textures;

  std::map<std::pair<ReflectorTexture, ReflectorTexture>, ReflectorTexture>
      m_product_reflector_textures;
  std::map<std::pair<FloatTexture, FloatTexture>, FloatTexture>
      m_product_float_textures;
};

}  // namespace iris

#endif  // _SRC_COMMON_TEXTURE_MANAGER_