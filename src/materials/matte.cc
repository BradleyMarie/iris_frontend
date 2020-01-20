#include "src/materials/matte.h"

#include "src/param_matchers/matcher.h"
#include "src/param_matchers/reflector_texture.h"

namespace iris {
namespace {

static const float_t kMatteMaterialDefaultReflectance = (float_t)0.5;

template <typename T>
MaterialFactory ParseMatte(const char* base_type_name, const char* type_name,
                           T& parameters,
                           const NamedTextureManager& named_texture_manager,
                           TextureManager& texture_manager,
                           SpectrumManager& spectrum_manager) {
  ReflectorTextureMatcher kd = ReflectorTextureMatcher::FromUniformReflectance(
      base_type_name, type_name, "Kd", false, named_texture_manager,
      texture_manager, spectrum_manager, kMatteMaterialDefaultReflectance);
  MatchParameters<1>(base_type_name, type_name, parameters, {&kd});

  ReflectorTexture default_kd = kd.Get();
  MaterialFactoryFn result =
      [default_kd](const char* base_type_name, const char* type_name,
                   std::vector<Parameter>& parameters,
                   MaterialManager& material_manager,
                   const NamedTextureManager& named_texture_manager,
                   TextureManager& texture_manager,
                   SpectrumManager& spectrum_manager) -> Material {
    ReflectorTextureMatcher kd(base_type_name, type_name, "Kd", false,
                               named_texture_manager, texture_manager,
                               spectrum_manager, default_kd);
    MatchParameters<1>(base_type_name, type_name, parameters, {&kd});

    return material_manager.AllocateMatteMaterial(kd.Get());
  };

  return MaterialFactory(std::move(result));
}

}  // namespace

MaterialFactory ParseMatte(const char* base_type_name, const char* type_name,
                           Tokenizer& tokenizer,
                           const NamedTextureManager& named_texture_manager,
                           TextureManager& texture_manager,
                           SpectrumManager& spectrum_manager) {
  return ParseMatte<Tokenizer>(base_type_name, type_name, tokenizer,
                               named_texture_manager, texture_manager,
                               spectrum_manager);
}

MaterialFactory MakeNamedMatte(const char* base_type_name,
                               const char* type_name,
                               std::vector<Parameter>& parameters,
                               const NamedTextureManager& named_texture_manager,
                               TextureManager& texture_manager,
                               SpectrumManager& spectrum_manager) {
  return ParseMatte<typename std::vector<Parameter>>(
      base_type_name, type_name, parameters, named_texture_manager,
      texture_manager, spectrum_manager);
}

}  // namespace iris