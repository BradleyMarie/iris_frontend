#ifndef _SRC_COMMON_MATERIAL_MANAGER_
#define _SRC_COMMON_MATERIAL_MANAGER_

#include <map>

#include "absl/container/flat_hash_map.h"
#include "absl/strings/string_view.h"
#include "src/common/pointer_types.h"

namespace iris {

class MaterialManager {
 public:
  const Material& GetMaterial(const absl::string_view name) const;
  void SetMaterial(absl::string_view name, const Material& material);

  Material AllocateMatteMaterial(const ReflectorTexture& kd);

 private:
  absl::flat_hash_map<std::string, Material> m_materials;
  std::map<ReflectorTexture, Material> m_matte_materials;
};

}  // namespace iris

#endif  // _SRC_COMMON_MATERIAL_MANAGER_