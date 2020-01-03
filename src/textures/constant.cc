#include "src/textures/constant.h"

#include "src/param_matchers/float_texture.h"
#include "src/param_matchers/matcher.h"
#include "src/param_matchers/reflector_texture.h"

namespace iris {
namespace {

static const float_t kConstantTextureDefaultValue = (float_t)1.0;

}  // namespace

std::pair<ReflectorTexture, std::set<Reflector>> ParseConstantReflector(
    const char* base_type_name, const char* type_name, Tokenizer& tokenizer,
    const ColorExtrapolator& color_extrapolator,
    const TextureManager& texture_manager) {
  ReflectorTextureMatcher value =
      ReflectorTextureMatcher::FromUniformReflectance(
          base_type_name, type_name, "value", false, texture_manager,
          color_extrapolator, kConstantTextureDefaultValue);

  MatchParameters<1>(base_type_name, type_name, tokenizer, {&value});

  return value.Get();
}

FloatTexture ParseConstantFloat(const char* base_type_name,
                                const char* type_name, Tokenizer& tokenizer,
                                const TextureManager& texture_manager) {
  FloatTextureMatcher value = FloatTextureMatcher::FromValue(
      base_type_name, type_name, "value", false, true, (float_t)0.0,
      (float_t)1.0, texture_manager, kConstantTextureDefaultValue);

  MatchParameters<1>(base_type_name, type_name, tokenizer, {&value});

  return value.Get();
}

}  // namespace iris