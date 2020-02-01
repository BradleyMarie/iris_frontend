#include "src/textures/scale.h"

#include "src/param_matchers/float_texture.h"
#include "src/param_matchers/matcher.h"
#include "src/param_matchers/reflector_texture.h"

namespace iris {
namespace {

static const float_t kScaleTextureDefaultValue = (float_t)1.0;

}  // namespace

ReflectorTexture ParseScaleReflector(
    const char* base_type_name, const char* type_name, Tokenizer& tokenizer,
    const NamedTextureManager& named_texture_manager,
    TextureManager& texture_manager, SpectrumManager& spectrum_manager) {
  ReflectorTextureMatcher tex1 =
      ReflectorTextureMatcher::FromUniformReflectance(
          base_type_name, type_name, "tex1", false, named_texture_manager,
          texture_manager, spectrum_manager, kScaleTextureDefaultValue);
  ReflectorTextureMatcher tex2 =
      ReflectorTextureMatcher::FromUniformReflectance(
          base_type_name, type_name, "tex2", false, named_texture_manager,
          texture_manager, spectrum_manager, kScaleTextureDefaultValue);
  MatchParameters<2>(base_type_name, type_name, tokenizer, {&tex1, &tex2});

  return texture_manager.AllocateProductReflectorTexture(tex1.Get(),
                                                         tex2.Get());
}

FloatTexture ParseScaleFloat(const char* base_type_name, const char* type_name,
                             Tokenizer& tokenizer,
                             const NamedTextureManager& named_texture_manager,
                             TextureManager& texture_manager) {
  FloatTextureMatcher tex1 = FloatTextureMatcher::FromValue(
      base_type_name, type_name, "tex1", false, true, (float_t)0.0,
      (float_t)1.0, named_texture_manager, texture_manager,
      kScaleTextureDefaultValue);
  FloatTextureMatcher tex2 = FloatTextureMatcher::FromValue(
      base_type_name, type_name, "tex2", false, true, (float_t)0.0,
      (float_t)1.0, named_texture_manager, texture_manager,
      kScaleTextureDefaultValue);

  MatchParameters<2>(base_type_name, type_name, tokenizer, {&tex1, &tex2});

  return texture_manager.AllocateProductFloatTexture(tex1.Get(), tex2.Get());
}

}  // namespace iris