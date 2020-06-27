#include "src/materials/matte.h"

#include "src/param_matchers/float_texture.h"
#include "src/param_matchers/matcher.h"
#include "src/param_matchers/reflector_texture.h"

namespace iris {
namespace {

static const float_t kMatteMaterialDefaultReflectance = (float_t)0.5;
static const float_t kMatteMaterialDefaultSigma = (float_t)0.0;
static const FloatTexture kMatteMaterialDefaultBumpMap;

template <typename T>
MaterialFactory ParseMatte(const char* base_type_name, const char* type_name,
                           T& parameters,
                           const NamedTextureManager& named_texture_manager,
                           NormalMapManager& normal_map_manager,
                           TextureManager& texture_manager,
                           SpectrumManager& spectrum_manager) {
  ReflectorTextureMatcher kd = ReflectorTextureMatcher::FromUniformReflectance(
      base_type_name, type_name, "Kd", false, named_texture_manager,
      texture_manager, spectrum_manager, kMatteMaterialDefaultReflectance);
  FloatTextureMatcher sigma = FloatTextureMatcher::FromValue(
      base_type_name, type_name, "sigma", false, false,
      -std::numeric_limits<float_t>::infinity(),
      std::numeric_limits<float_t>::infinity(), named_texture_manager,
      texture_manager, kMatteMaterialDefaultSigma);
  FloatTextureMatcher bumpmap(base_type_name, type_name, "bumpmap", false, true,
                              (float_t)0.0, (float_t)1.0, named_texture_manager,
                              texture_manager, kMatteMaterialDefaultBumpMap);
  MatchParameters<3>(base_type_name, type_name, parameters,
                     {&kd, &sigma, &bumpmap});

  ReflectorTexture default_kd = kd.Get();
  FloatTexture default_sigma = sigma.Get();
  FloatTexture default_bumpmap = bumpmap.Get();
  MaterialFactoryFn result =
      [default_kd, default_sigma, default_bumpmap](
          const char* base_type_name, const char* type_name,
          std::vector<Parameter>& parameters, MaterialManager& material_manager,
          const NamedTextureManager& named_texture_manager,
          NormalMapManager& normal_map_manager, TextureManager& texture_manager,
          SpectrumManager& spectrum_manager) -> std::pair<Material, NormalMap> {
    ReflectorTextureMatcher kd(base_type_name, type_name, "Kd", false,
                               named_texture_manager, texture_manager,
                               spectrum_manager, default_kd);
    FloatTextureMatcher sigma(base_type_name, type_name, "sigma", false, false,
                              -std::numeric_limits<float_t>::infinity(),
                              std::numeric_limits<float_t>::infinity(),
                              named_texture_manager, texture_manager,
                              default_sigma);
    FloatTextureMatcher bumpmap(
        base_type_name, type_name, "bumpmap", false, true, (float_t)0.0,
        (float_t)1.0, named_texture_manager, texture_manager, default_bumpmap);
    MatchParameters<3>(base_type_name, type_name, parameters,
                       {&kd, &sigma, &bumpmap});

    return std::make_pair(
        material_manager.AllocateMatteMaterial(kd.Get(), sigma.Get()),
        normal_map_manager.AllocateBumpMap(bumpmap.Get()));
  };

  return MaterialFactory(std::move(result));
}

}  // namespace

MaterialFactory ParseMatte(const char* base_type_name, const char* type_name,
                           Tokenizer& tokenizer,
                           const NamedTextureManager& named_texture_manager,
                           NormalMapManager& normal_map_manager,
                           TextureManager& texture_manager,
                           SpectrumManager& spectrum_manager) {
  return ParseMatte<Tokenizer>(base_type_name, type_name, tokenizer,
                               named_texture_manager, normal_map_manager,
                               texture_manager, spectrum_manager);
}

MaterialFactory MakeNamedMatte(const char* base_type_name,
                               const char* type_name,
                               std::vector<Parameter>& parameters,
                               const NamedTextureManager& named_texture_manager,
                               NormalMapManager& normal_map_manager,
                               TextureManager& texture_manager,
                               SpectrumManager& spectrum_manager) {
  return ParseMatte<typename std::vector<Parameter>>(
      base_type_name, type_name, parameters, named_texture_manager,
      normal_map_manager, texture_manager, spectrum_manager);
}

}  // namespace iris