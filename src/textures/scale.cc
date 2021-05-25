#include "src/textures/scale.h"

#include "src/param_matchers/float_texture.h"
#include "src/param_matchers/reflector_texture.h"

namespace iris {
namespace {

static const float_t kScaleTextureDefaultValue = (float_t)1.0;

}  // namespace

ReflectorTexture ParseScaleReflector(
    Parameters& parameters, const Matrix& texture_to_world,
    const NamedTextureManager& named_texture_manager,
    TextureManager& texture_manager, SpectrumManager& spectrum_manager) {
  ReflectorTextureMatcher tex1 =
      ReflectorTextureMatcher::FromUniformReflectance(
          "tex1", false, named_texture_manager, texture_manager,
          spectrum_manager, kScaleTextureDefaultValue);
  ReflectorTextureMatcher tex2 =
      ReflectorTextureMatcher::FromUniformReflectance(
          "tex2", false, named_texture_manager, texture_manager,
          spectrum_manager, kScaleTextureDefaultValue);
  parameters.Match(tex1, tex2);

  return texture_manager.AllocateProductReflectorTexture(tex1.Get(),
                                                         tex2.Get());
}

FloatTexture ParseScaleFloat(Parameters& parameters,
                             const Matrix& texture_to_world,
                             const NamedTextureManager& named_texture_manager,
                             TextureManager& texture_manager) {
  FloatTextureMatcher tex1 = FloatTextureMatcher::FromValue(
      "tex1", false, true, (float_t)0.0, (float_t)1.0, named_texture_manager,
      texture_manager, kScaleTextureDefaultValue);
  FloatTextureMatcher tex2 = FloatTextureMatcher::FromValue(
      "tex2", false, true, (float_t)0.0, (float_t)1.0, named_texture_manager,
      texture_manager, kScaleTextureDefaultValue);
  parameters.Match(tex1, tex2);

  return texture_manager.AllocateProductFloatTexture(tex1.Get(), tex2.Get());
}

}  // namespace iris