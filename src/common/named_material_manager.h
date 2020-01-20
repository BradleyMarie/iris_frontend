#ifndef _SRC_COMMON_NAMED_MATERIAL_MANAGER_
#define _SRC_COMMON_NAMED_MATERIAL_MANAGER_

#include "absl/container/flat_hash_map.h"
#include "absl/strings/string_view.h"
#include "src/common/material_factory.h"
#include "src/common/pointer_types.h"

namespace iris {

class NamedMaterialManager {
 public:
  const MaterialFactory& GetMaterial(const absl::string_view name) const;
  void SetMaterial(absl::string_view name, const MaterialFactory& material);

 private:
  absl::flat_hash_map<std::string, MaterialFactory> m_materials;
};

}  // namespace iris

#endif  // _SRC_COMMON_NAMED_MATERIAL_MANAGER_