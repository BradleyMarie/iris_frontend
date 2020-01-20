#include "src/common/named_material_manager.h"

#include <iostream>

namespace iris {

const MaterialFactory& NamedMaterialManager::GetMaterial(
    absl::string_view name) const {
  auto iter = m_materials.find(name);
  if (iter == m_materials.end()) {
    std::cerr << "ERROR: Material not defined: " << name << std::endl;
    exit(EXIT_FAILURE);
  }
  return iter->second;
}

void NamedMaterialManager::SetMaterial(const absl::string_view name,
                                       const MaterialFactory& material) {
  m_materials[name] = material;
}

}  // namespace iris