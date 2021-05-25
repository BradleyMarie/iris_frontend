#include "src/materials/mirror.h"

#include "src/param_matchers/float_texture.h"
#include "src/param_matchers/reflector_texture.h"

namespace iris {
namespace {

static const float_t kMirrorMaterialDefaultReflectance = (float_t)0.5;
static const FloatTexture kMirrorMaterialDefaultBumpMap;

}  // namespace

MaterialResult ParseMirror(Parameters& parameters,
                           const NamedTextureManager& named_texture_manager,
                           NormalMapManager& normal_map_manager,
                           TextureManager& texture_manager,
                           SpectrumManager& spectrum_manager) {
  ReflectorTextureMatcher kr = ReflectorTextureMatcher::FromUniformReflectance(
      "Kr", false, named_texture_manager, texture_manager, spectrum_manager,
      kMirrorMaterialDefaultReflectance);
  FloatTextureMatcher bumpmap("bumpmap", false, true, (float_t)0.0,
                              (float_t)1.0, named_texture_manager,
                              texture_manager, kMirrorMaterialDefaultBumpMap);
  parameters.Match(kr, bumpmap);

  ReflectorTexture default_kr = kr.Get();
  FloatTexture default_bumpmap = bumpmap.Get();
  return
      [default_kr, default_bumpmap](
          Parameters& parameters, MaterialManager& material_manager,
          const NamedTextureManager& named_texture_manager,
          NormalMapManager& normal_map_manager, TextureManager& texture_manager,
          SpectrumManager& spectrum_manager) -> std::pair<Material, NormalMap> {
        ReflectorTextureMatcher kr("Kr", false, named_texture_manager,
                                   texture_manager, spectrum_manager,
                                   default_kr);
        FloatTextureMatcher bumpmap("bumpmap", false, true, (float_t)0.0,
                                    (float_t)1.0, named_texture_manager,
                                    texture_manager, default_bumpmap);
        parameters.Match(kr, bumpmap);

        return std::make_pair(
            material_manager.AllocateMirrorMaterial(kr.Get()),
            normal_map_manager.AllocateBumpMap(bumpmap.Get()));
      };
}

}  // namespace iris