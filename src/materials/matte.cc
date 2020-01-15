#include "src/materials/matte.h"

#include "src/materials/matcher.h"
#include "src/param_matchers/reflector_texture.h"

namespace iris {
namespace {

static const float_t kMatteMaterialDefaultReflectance = (float_t)0.5;

}  // namespace

Material ParseMatte(const char* base_type_name, const char* type_name,
                    Tokenizer& tokenizer, MaterialManager& material_manager,
                    const NamedTextureManager& named_texture_manager,
                    TextureManager& texture_manager,
                    SpectrumManager& spectrum_manager, bool ignore_type) {
  ReflectorTextureMatcher kd = ReflectorTextureMatcher::FromUniformReflectance(
      base_type_name, type_name, "Kd", false, named_texture_manager,
      texture_manager, spectrum_manager, kMatteMaterialDefaultReflectance);
  MatchMaterialParameters<1>(base_type_name, type_name, tokenizer, ignore_type,
                             {&kd});

  return material_manager.AllocateMatteMaterial(kd.Get());
}

}  // namespace iris