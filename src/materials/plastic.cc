#include "src/materials/plastic.h"

#include "src/param_matchers/float_texture.h"
#include "src/param_matchers/matcher.h"
#include "src/param_matchers/reflector_texture.h"
#include "src/param_matchers/single.h"

namespace iris {
namespace {

static const float_t kPlasticMaterialDefaultDiffuse = (float_t)0.25;
static const float_t kPlasticMaterialDefaultSpecular = (float_t)0.25;
static const float_t kPlasticMaterialDefaultRoughness = (float_t)0.1;
static const bool kPlasticMaterialDefaultRemapRoughness = true;
static const FloatTexture kPlasticMaterialDefaultBumpMap;

template <typename T>
MaterialFactory ParsePlastic(const char* base_type_name, const char* type_name,
                             T& parameters,
                             const NamedTextureManager& named_texture_manager,
                             NormalMapManager& normal_map_manager,
                             TextureManager& texture_manager,
                             SpectrumManager& spectrum_manager) {
  ReflectorTextureMatcher kd = ReflectorTextureMatcher::FromUniformReflectance(
      base_type_name, type_name, "Kd", false, named_texture_manager,
      texture_manager, spectrum_manager, kPlasticMaterialDefaultDiffuse);
  ReflectorTextureMatcher ks = ReflectorTextureMatcher::FromUniformReflectance(
      base_type_name, type_name, "Ks", false, named_texture_manager,
      texture_manager, spectrum_manager, kPlasticMaterialDefaultSpecular);
  FloatTextureMatcher roughness = FloatTextureMatcher::FromValue(
      base_type_name, type_name, "roughness", false, true,
      static_cast<float_t>(0.0), static_cast<float_t>(1.0),
      named_texture_manager, texture_manager, kPlasticMaterialDefaultRoughness);
  SingleBoolMatcher remap_roughness(base_type_name, type_name,
                                    "reamaproughness", false,
                                    kPlasticMaterialDefaultRemapRoughness);
  FloatTextureMatcher bumpmap(base_type_name, type_name, "bumpmap", false, true,
                              (float_t)0.0, (float_t)1.0, named_texture_manager,
                              texture_manager, kPlasticMaterialDefaultBumpMap);
  MatchParameters<5>(base_type_name, type_name, parameters,
                     {&kd, &ks, &roughness, &remap_roughness, &bumpmap});

  ReflectorTexture default_kd = kd.Get();
  ReflectorTexture default_ks = ks.Get();
  FloatTexture default_roughness = roughness.Get();
  bool default_remap_roughness = remap_roughness.Get();
  FloatTexture default_bumpmap = bumpmap.Get();
  MaterialFactoryFn result =
      [default_kd, default_ks, default_roughness, default_remap_roughness,
       default_bumpmap](
          const char* base_type_name, const char* type_name,
          std::vector<Parameter>& parameters, MaterialManager& material_manager,
          const NamedTextureManager& named_texture_manager,
          NormalMapManager& normal_map_manager, TextureManager& texture_manager,
          SpectrumManager& spectrum_manager) -> std::pair<Material, NormalMap> {
    ReflectorTextureMatcher kd(base_type_name, type_name, "Kd", false,
                               named_texture_manager, texture_manager,
                               spectrum_manager, default_kd);
    ReflectorTextureMatcher ks(base_type_name, type_name, "Ks", false,
                               named_texture_manager, texture_manager,
                               spectrum_manager, default_ks);
    FloatTextureMatcher roughness(
        base_type_name, type_name, "roughness", false, true,
        static_cast<float_t>(0.0), static_cast<float_t>(1.0),
        named_texture_manager, texture_manager, default_roughness);
    SingleBoolMatcher remap_roughness(base_type_name, type_name,
                                      "reamaproughness", false,
                                      default_remap_roughness);
    FloatTextureMatcher bumpmap(
        base_type_name, type_name, "bumpmap", false, true, (float_t)0.0,
        (float_t)1.0, named_texture_manager, texture_manager, default_bumpmap);
    MatchParameters<5>(base_type_name, type_name, parameters,
                       {&kd, &ks, &roughness, &remap_roughness, &bumpmap});

    return std::make_pair(
        material_manager.AllocatePlasticMaterial(
            kd.Get(), ks.Get(), roughness.Get(), remap_roughness.Get()),
        normal_map_manager.AllocateBumpMap(bumpmap.Get()));
  };

  return MaterialFactory(std::move(result));
}

}  // namespace

MaterialFactory ParsePlastic(const char* base_type_name, const char* type_name,
                             Tokenizer& tokenizer,
                             const NamedTextureManager& named_texture_manager,
                             NormalMapManager& normal_map_manager,
                             TextureManager& texture_manager,
                             SpectrumManager& spectrum_manager) {
  return ParsePlastic<Tokenizer>(base_type_name, type_name, tokenizer,
                                 named_texture_manager, normal_map_manager,
                                 texture_manager, spectrum_manager);
}

MaterialFactory MakeNamedPlastic(
    const char* base_type_name, const char* type_name,
    std::vector<Parameter>& parameters,
    const NamedTextureManager& named_texture_manager,
    NormalMapManager& normal_map_manager, TextureManager& texture_manager,
    SpectrumManager& spectrum_manager) {
  return ParsePlastic<typename std::vector<Parameter>>(
      base_type_name, type_name, parameters, named_texture_manager,
      normal_map_manager, texture_manager, spectrum_manager);
}

}  // namespace iris