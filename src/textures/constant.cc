#include "src/textures/constant.h"

#include "src/param_matchers/float_texture.h"
#include "src/param_matchers/reflector_texture.h"

namespace iris {
namespace {

static const float_t kConstantTextureDefaultValue = (float_t)1.0;

}  // namespace

ReflectorTexture ParseConstantReflector(
    Parameters& parameters, const NamedTextureManager& named_texture_manager,
    TextureManager& texture_manager, SpectrumManager& spectrum_manager) {
  ReflectorTextureMatcher value =
      ReflectorTextureMatcher::FromUniformReflectance(
          "value", false, named_texture_manager, texture_manager,
          spectrum_manager, kConstantTextureDefaultValue);
  parameters.Match(value);

  return value.Get();
}

FloatTexture ParseConstantFloat(
    Parameters& parameters, const NamedTextureManager& named_texture_manager,
    TextureManager& texture_manager) {
  FloatTextureMatcher value = FloatTextureMatcher::FromValue(
      "value", false, true, (float_t)0.0, (float_t)1.0, named_texture_manager,
      texture_manager, kConstantTextureDefaultValue);
  parameters.Match(value);

  return value.Get();
}

}  // namespace iris