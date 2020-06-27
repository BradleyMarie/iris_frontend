#ifndef _SRC_COMMON_MATERIAL_MANAGER_
#define _SRC_COMMON_MATERIAL_MANAGER_

#include <map>

#include "src/common/pointer_types.h"

namespace iris {

class MaterialManager {
 public:
  const Material& AllocateMatteMaterial(const ReflectorTexture& kd,
                                        const FloatTexture& sigma);

 private:
  std::map<std::pair<ReflectorTexture, FloatTexture>, Material>
      m_matte_materials;
};

}  // namespace iris

#endif  // _SRC_COMMON_MATERIAL_MANAGER_