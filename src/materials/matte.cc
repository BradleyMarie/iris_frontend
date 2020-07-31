#include "src/materials/matte.h"

#include "src/param_matchers/float_texture.h"
#include "src/param_matchers/matcher.h"
#include "src/param_matchers/reflector_texture.h"

namespace iris {
namespace {

static const float_t kMatteMaterialDefaultReflectance = (float_t)0.5;
static const float_t kMatteMaterialDefaultSigma = (float_t)0.0;
static const FloatTexture kMatteMaterialDefaultBumpMap;

}  // namespace

MaterialFactory ParseMatte(Parameters& parameters,
                           const NamedTextureManager& named_texture_manager,
                           NormalMapManager& normal_map_manager,
                           TextureManager& texture_manager,
                           SpectrumManager& spectrum_manager) {
  ReflectorTextureMatcher kd = ReflectorTextureMatcher::FromUniformReflectance(
      "Kd", false, named_texture_manager, texture_manager, spectrum_manager,
      kMatteMaterialDefaultReflectance);
  FloatTextureMatcher sigma = FloatTextureMatcher::FromValue(
      "sigma", false, false, -std::numeric_limits<float_t>::infinity(),
      std::numeric_limits<float_t>::infinity(), named_texture_manager,
      texture_manager, kMatteMaterialDefaultSigma);
  FloatTextureMatcher bumpmap("bumpmap", false, true, (float_t)0.0,
                              (float_t)1.0, named_texture_manager,
                              texture_manager, kMatteMaterialDefaultBumpMap);
  parameters.Match(kd, sigma, bumpmap);

  ReflectorTexture default_kd = kd.Get();
  FloatTexture default_sigma = sigma.Get();
  FloatTexture default_bumpmap = bumpmap.Get();
  return
      [default_kd, default_sigma, default_bumpmap](
          Parameters& parameters, MaterialManager& material_manager,
          const NamedTextureManager& named_texture_manager,
          NormalMapManager& normal_map_manager, TextureManager& texture_manager,
          SpectrumManager& spectrum_manager) -> std::pair<Material, NormalMap> {
        ReflectorTextureMatcher kd("Kd", false, named_texture_manager,
                                   texture_manager, spectrum_manager,
                                   default_kd);
        FloatTextureMatcher sigma(
            "sigma", false, false, -std::numeric_limits<float_t>::infinity(),
            std::numeric_limits<float_t>::infinity(), named_texture_manager,
            texture_manager, default_sigma);
        FloatTextureMatcher bumpmap("bumpmap", false, true, (float_t)0.0,
                                    (float_t)1.0, named_texture_manager,
                                    texture_manager, default_bumpmap);
        parameters.Match(kd, sigma, bumpmap);

        return std::make_pair(
            material_manager.AllocateMatteMaterial(kd.Get(), sigma.Get()),
            normal_map_manager.AllocateBumpMap(bumpmap.Get()));
      };
}

}  // namespace iris