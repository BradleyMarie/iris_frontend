#ifndef _SRC_COMMON_MATERIAL_FACTORY_
#define _SRC_COMMON_MATERIAL_FACTORY_

#include <functional>
#include <vector>

#include "src/common/material_manager.h"
#include "src/common/named_texture_manager.h"
#include "src/common/spectrum_manager.h"
#include "src/common/texture_manager.h"
#include "src/param_matchers/parser.h"

namespace iris {

typedef std::function<Material(
    const char* base_type_name, const char* type_name, std::vector<Parameter>&,
    MaterialManager& material_manager,
    const NamedTextureManager& named_texture_manager,
    TextureManager& texture_manager, SpectrumManager& spectrum_manager)>
    MaterialFactoryFn;

class MaterialFactory {
 public:
  MaterialFactory() = default;
  MaterialFactory(MaterialFactoryFn&& factory)
      : m_factory(std::move(factory)) {}

  Material Build(const char* base_type_name, const char* type_name,
                 std::vector<Parameter>& param_overrides,
                 MaterialManager& material_manager,
                 const NamedTextureManager& named_texture_manager,
                 TextureManager& texture_manager,
                 SpectrumManager& spectrum_manager) const {
    if (!m_factory) {
      return Material();
    }

    return m_factory(base_type_name, type_name, param_overrides,
                     material_manager, named_texture_manager, texture_manager,
                     spectrum_manager);
  }

 private:
  MaterialFactoryFn m_factory;
};

}  // namespace iris

#endif  // _SRC_COMMON_MATERIAL_FACTORY_