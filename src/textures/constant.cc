#include "src/textures/constant.h"

#include "src/param_matchers/float_texture.h"
#include "src/param_matchers/matcher.h"
#include "src/param_matchers/reflector_texture.h"

namespace iris {
namespace {

static const float_t kConstantTextureDefaultValue = (float_t)1.0;

}  // namespace

ReflectorTexture ParseConstantReflector(
    const char* base_type_name, const char* type_name, Tokenizer& tokenizer,
    const NamedTextureManager& named_texture_manager,
    TextureManager& texture_manager, SpectrumManager& spectrum_manager) {
  ReflectorTextureMatcher value =
      ReflectorTextureMatcher::FromUniformReflectance(
          "value", false, tokenizer, named_texture_manager, texture_manager,
          spectrum_manager, kConstantTextureDefaultValue);
  MatchParameters(base_type_name, type_name, tokenizer, {&value});

  return value.Get();
}

FloatTexture ParseConstantFloat(
    const char* base_type_name, const char* type_name, Tokenizer& tokenizer,
    const NamedTextureManager& named_texture_manager,
    TextureManager& texture_manager) {
  FloatTextureMatcher value = FloatTextureMatcher::FromValue(
      "value", false, true, (float_t)0.0, (float_t)1.0, named_texture_manager,
      texture_manager, kConstantTextureDefaultValue);

  MatchParameters(base_type_name, type_name, tokenizer, {&value});

  return value.Get();
}

}  // namespace iris