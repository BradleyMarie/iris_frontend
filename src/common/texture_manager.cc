#include "src/common/texture_manager.h"

#include "iris_physx_toolkit/constant_texture.h"
#include "iris_physx_toolkit/image_texture.h"
#include "iris_physx_toolkit/perlin_textures.h"
#include "iris_physx_toolkit/product_texture.h"
#include "src/common/error.h"

namespace iris {

const ReflectorTexture& TextureManager::AllocateConstantReflectorTexture(
    const Reflector& reflector) {
  ReflectorTexture& result = m_constant_reflector_textures[reflector];
  if (!result.get()) {
    ISTATUS status = ConstantReflectorTextureAllocate(
        reflector.get(), result.release_and_get_address());
    SuccessOrOOM(status);
  }

  return result;
}

const FloatTexture& TextureManager::AllocateConstantFloatTexture(
    float_t value) {
  FloatTexture& result = m_constant_float_textures[value];
  if (!result.get()) {
    ISTATUS status =
        ConstantFloatTextureAllocate(value, result.release_and_get_address());
    SuccessOrOOM(status);
  }

  return result;
}

const ReflectorTexture& TextureManager::AllocateProductReflectorTexture(
    const ReflectorTexture& tex1, const ReflectorTexture& tex2) {
  ReflectorTexture& result =
      m_product_reflector_textures[std::make_pair(tex1, tex2)];
  if (!result.get()) {
    ISTATUS status = ProductReflectorTextureAllocate(
        tex1.get(), tex2.get(), result.release_and_get_address());
    SuccessOrOOM(status);
  }

  return result;
}

const FloatTexture& TextureManager::AllocateProductFloatTexture(
    const FloatTexture& tex1, const FloatTexture& tex2) {
  FloatTexture& result = m_product_float_textures[std::make_pair(tex1, tex2)];
  if (!result.get()) {
    ISTATUS status = ProductFloatTextureAllocate(
        tex1.get(), tex2.get(), result.release_and_get_address());
    SuccessOrOOM(status);
  }

  return result;
}

ReflectorTexture TextureManager::AllocateImageMapReflectorTexture(
    ReflectorMipmap mipmap, float_t u_delta, float_t v_delta, float_t u_scale,
    float_t v_scale) {
  ReflectorTexture result;
  ISTATUS status =
      ImageReflectorTextureAllocate(mipmap.detach(), u_delta, -v_delta, u_scale,
                                    -v_scale, result.release_and_get_address());
  SuccessOrOOM(status);

  return result;
}

FloatTexture TextureManager::AllocateImageMapFloatTexture(FloatMipmap mipmap,
                                                          float_t u_delta,
                                                          float_t v_delta,
                                                          float_t u_scale,
                                                          float_t v_scale) {
  FloatTexture result;
  ISTATUS status =
      ImageFloatTextureAllocate(mipmap.detach(), u_delta, -v_delta, u_scale,
                                -v_scale, result.release_and_get_address());
  SuccessOrOOM(status);

  return result;
}

const ReflectorTexture& TextureManager::AllocateWindyReflectorTexture(
    const Matrix& texture_to_world, const Reflector& reflector) {
  ReflectorTexture& result =
      m_windy_reflector_textures[std::make_pair(texture_to_world, reflector)];
  if (!result.get()) {
    ISTATUS status =
        WindyReflectorTextureAllocate(texture_to_world.get(), reflector.get(),
                                      result.release_and_get_address());
    SuccessOrOOM(status);
  }

  return result;
}

const FloatTexture& TextureManager::AllocateWindyFloatTexture(
    const Matrix& texture_to_world) {
  FloatTexture& result = m_windy_float_textures[texture_to_world];
  if (!result.get()) {
    ISTATUS status = WindyFloatTextureAllocate(
        texture_to_world.get(), result.release_and_get_address());
    SuccessOrOOM(status);
  }

  return result;
}

}  // namespace iris