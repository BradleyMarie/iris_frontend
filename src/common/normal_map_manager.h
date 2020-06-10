#ifndef _SRC_COMMON_NORMAL_MAP_MANAGER_
#define _SRC_COMMON_NORMAL_MAP_MANAGER_

#include <map>

#include "src/common/pointer_types.h"

namespace iris {

class NormalMapManager {
 public:
  const NormalMap& AllocateBumpMap(const FloatTexture& texture);

 private:
  std::map<FloatTexture, NormalMap> m_bump_maps;
};

}  // namespace iris

#endif  // _SRC_COMMON_NORMAL_MAP_MANAGER_