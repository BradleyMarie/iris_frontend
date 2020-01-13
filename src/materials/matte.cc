#include "src/materials/matte.h"

#include "iris_physx_toolkit/matte_material.h"
#include "src/common/error.h"
#include "src/param_matchers/matcher.h"
#include "src/param_matchers/reflector_texture.h"

namespace iris {
namespace {

static const float_t kMatteMaterialDefaultReflectance = (float_t)0.5;

}  // namespace

Material ParseMatte(const char* base_type_name, const char* type_name,
                    Tokenizer& tokenizer, SpectrumManager& spectrum_manager,
                    const TextureManager& texture_manager) {
  ReflectorTextureMatcher kd = ReflectorTextureMatcher::FromUniformReflectance(
      base_type_name, type_name, "Kd", false, texture_manager, spectrum_manager,
      kMatteMaterialDefaultReflectance);
  MatchParameters<1>(base_type_name, type_name, tokenizer, {&kd});

  Material result;
  ISTATUS status =
      MatteMaterialAllocate(kd.Get().get(), result.release_and_get_address());
  SuccessOrOOM(status);

  return result;
}

}  // namespace iris