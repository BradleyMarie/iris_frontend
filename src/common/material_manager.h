#ifndef _SRC_COMMON_MATERIAL_MANAGER_
#define _SRC_COMMON_MATERIAL_MANAGER_

#include <map>

#include "src/common/pointer_types.h"

namespace iris {

class MaterialManager {
 public:
  Material AllocateMatteMaterial(const ReflectorTexture& kd);

 private:
  std::map<ReflectorTexture, Material> m_matte_materials;
};

}  // namespace iris

#endif  // _SRC_COMMON_MATERIAL_MANAGER_