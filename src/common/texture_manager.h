#ifndef _SRC_COMMON_TEXTURE_MANAGER_
#define _SRC_COMMON_TEXTURE_MANAGER_

#include <map>

#include "src/common/pointer_types.h"

namespace iris {

class TextureManager {
 public:
  const ReflectorTexture& AllocateConstantReflectorTexture(
      const Reflector& reflector);
  const FloatTexture& AllocateConstantFloatTexture(float_t value);

  const ReflectorTexture& AllocateProductReflectorTexture(
      const ReflectorTexture& tex1, const ReflectorTexture& tex2);
  const FloatTexture& AllocateProductFloatTexture(const FloatTexture& tex1,
                                                  const FloatTexture& tex2);

  ReflectorTexture AllocateImageMapReflectorTexture(ReflectorMipmap mipmap,
                                                    float_t u_delta,
                                                    float_t v_delta,
                                                    float_t u_scale,
                                                    float_t v_scale);
  FloatTexture AllocateImageMapFloatTexture(FloatMipmap mipmap, float_t u_delta,
                                            float_t v_delta, float_t u_scale,
                                            float_t v_scale);

  const ReflectorTexture& AllocateWindyReflectorTexture(
      const Matrix& texture_to_world, const Reflector& reflector);
  const FloatTexture& AllocateWindyFloatTexture(const Matrix& texture_to_world);

 private:
  std::map<Reflector, ReflectorTexture> m_constant_reflector_textures;
  std::map<float_t, FloatTexture> m_constant_float_textures;

  std::map<std::pair<ReflectorTexture, ReflectorTexture>, ReflectorTexture>
      m_product_reflector_textures;
  std::map<std::pair<FloatTexture, FloatTexture>, FloatTexture>
      m_product_float_textures;

  std::map<std::pair<Matrix, Reflector>, ReflectorTexture>
      m_windy_reflector_textures;
  std::map<Matrix, FloatTexture> m_windy_float_textures;
};

}  // namespace iris

#endif  // _SRC_COMMON_TEXTURE_MANAGER_