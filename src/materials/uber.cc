#include "src/materials/uber.h"

#include "src/param_matchers/float_texture.h"
#include "src/param_matchers/reflector_texture.h"

namespace iris {
namespace {

static const float_t kUberMaterialDefaultReflectance = (float_t)0.5;
static const FloatTexture kUberMaterialDefaultBumpMap;

}  // namespace

MaterialResult ParseUber(Parameters& parameters,
                         const NamedTextureManager& named_texture_manager,
                         NormalMapManager& normal_map_manager,
                         TextureManager& texture_manager,
                         SpectrumManager& spectrum_manager) {
  ReflectorTextureMatcher kd = ReflectorTextureMatcher::FromUniformReflectance(
      "Kd", false, named_texture_manager, texture_manager, spectrum_manager,
      kUberMaterialDefaultReflectance);
  FloatTextureMatcher bumpmap("bumpmap", false, true, (float_t)0.0,
                              (float_t)1.0, named_texture_manager,
                              texture_manager, kUberMaterialDefaultBumpMap);
  parameters.Match(kd, bumpmap);

  ReflectorTexture default_kd = kd.Get();
  FloatTexture default_bumpmap = bumpmap.Get();
  return
      [default_kd, default_bumpmap](
          Parameters& parameters, MaterialManager& material_manager,
          const NamedTextureManager& named_texture_manager,
          NormalMapManager& normal_map_manager, TextureManager& texture_manager,
          SpectrumManager& spectrum_manager) -> std::pair<Material, NormalMap> {
        ReflectorTextureMatcher kd("Kd", false, named_texture_manager,
                                   texture_manager, spectrum_manager,
                                   default_kd);
        FloatTextureMatcher bumpmap("bumpmap", false, true, (float_t)0.0,
                                    (float_t)1.0, named_texture_manager,
                                    texture_manager, default_bumpmap);
        parameters.Match(kd, bumpmap);

        const FloatTexture& sigma =
            texture_manager.AllocateConstantFloatTexture((float_t)0.0);
        return std::make_pair(
            material_manager.AllocateMatteMaterial(kd.Get(), sigma),
            normal_map_manager.AllocateBumpMap(bumpmap.Get()));
      };
}

}  // namespace iris