#include "src/materials/plastic.h"

#include "src/param_matchers/float_texture.h"
#include "src/param_matchers/reflector_texture.h"
#include "src/param_matchers/single.h"

namespace iris {
namespace {

static const float_t kPlasticMaterialDefaultDiffuse = (float_t)0.25;
static const float_t kPlasticMaterialDefaultSpecular = (float_t)0.25;
static const float_t kPlasticMaterialDefaultRoughness = (float_t)0.1;
static const bool kPlasticMaterialDefaultRemapRoughness = true;
static const FloatTexture kPlasticMaterialDefaultBumpMap;

}  // namespace

MaterialFactory ParsePlastic(Parameters& parameters,
                             const NamedTextureManager& named_texture_manager,
                             NormalMapManager& normal_map_manager,
                             TextureManager& texture_manager,
                             SpectrumManager& spectrum_manager) {
  ReflectorTextureMatcher kd = ReflectorTextureMatcher::FromUniformReflectance(
      "Kd", false, named_texture_manager, texture_manager, spectrum_manager,
      kPlasticMaterialDefaultDiffuse);
  ReflectorTextureMatcher ks = ReflectorTextureMatcher::FromUniformReflectance(
      "Ks", false, named_texture_manager, texture_manager, spectrum_manager,
      kPlasticMaterialDefaultSpecular);
  FloatTextureMatcher roughness = FloatTextureMatcher::FromValue(
      "roughness", false, true, static_cast<float_t>(0.0),
      static_cast<float_t>(1.0), named_texture_manager, texture_manager,
      kPlasticMaterialDefaultRoughness);
  SingleBoolMatcher remap_roughness("reamaproughness", false,
                                    kPlasticMaterialDefaultRemapRoughness);
  FloatTextureMatcher bumpmap("bumpmap", false, true, (float_t)0.0,
                              (float_t)1.0, named_texture_manager,
                              texture_manager, kPlasticMaterialDefaultBumpMap);
  parameters.Match(kd, ks, roughness, remap_roughness, bumpmap);

  ReflectorTexture default_kd = kd.Get();
  ReflectorTexture default_ks = ks.Get();
  FloatTexture default_roughness = roughness.Get();
  bool default_remap_roughness = remap_roughness.Get();
  FloatTexture default_bumpmap = bumpmap.Get();
  return
      [default_kd, default_ks, default_roughness, default_remap_roughness,
       default_bumpmap](
          Parameters& parameters, MaterialManager& material_manager,
          const NamedTextureManager& named_texture_manager,
          NormalMapManager& normal_map_manager, TextureManager& texture_manager,
          SpectrumManager& spectrum_manager) -> std::pair<Material, NormalMap> {
        ReflectorTextureMatcher kd("Kd", false, named_texture_manager,
                                   texture_manager, spectrum_manager,
                                   default_kd);
        ReflectorTextureMatcher ks("Ks", false, named_texture_manager,
                                   texture_manager, spectrum_manager,
                                   default_ks);
        FloatTextureMatcher roughness(
            "roughness", false, true, static_cast<float_t>(0.0),
            static_cast<float_t>(1.0), named_texture_manager, texture_manager,
            default_roughness);
        SingleBoolMatcher remap_roughness("reamaproughness", false,
                                          default_remap_roughness);
        FloatTextureMatcher bumpmap("bumpmap", false, true, (float_t)0.0,
                                    (float_t)1.0, named_texture_manager,
                                    texture_manager, default_bumpmap);
        parameters.Match(kd, ks, roughness, remap_roughness, bumpmap);

        return std::make_pair(
            material_manager.AllocatePlasticMaterial(
                kd.Get(), ks.Get(), roughness.Get(), remap_roughness.Get()),
            normal_map_manager.AllocateBumpMap(bumpmap.Get()));
      };
}

}  // namespace iris