#ifndef _SRC_COMMON_MATERIAL_MANAGER_
#define _SRC_COMMON_MATERIAL_MANAGER_

#include <map>
#include <tuple>

#include "src/common/pointer_types.h"

namespace iris {

class MaterialManager {
 public:
  const Material& AllocateAlphaMaterial(const Material& material,
                                        const FloatTexture& alpha);
  const Material& AllocateMatteMaterial(const ReflectorTexture& kd,
                                        const FloatTexture& sigma);
  const Material& AllocateMirrorMaterial(const ReflectorTexture& kr);
  const Material& AllocatePlasticMaterial(const ReflectorTexture& kd,
                                          const ReflectorTexture& ks,
                                          const FloatTexture& roughness,
                                          bool remap_roughness);

 private:
  std::map<std::pair<Material, FloatTexture>, Material> m_alpha_materials;
  std::map<std::pair<ReflectorTexture, FloatTexture>, Material>
      m_matte_materials;
  std::map<ReflectorTexture, Material> m_mirror_materials;
  std::map<std::tuple<ReflectorTexture, ReflectorTexture, FloatTexture, bool>,
           Material>
      m_plastic_materials;
};

}  // namespace iris

#endif  // _SRC_COMMON_MATERIAL_MANAGER_