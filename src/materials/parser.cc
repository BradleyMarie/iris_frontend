#include "src/materials/parser.h"

#include <iostream>

#include "src/materials/matte.h"
#include "src/materials/plastic.h"

namespace iris {
namespace {

const Directive::Implementations<MaterialResult, const NamedTextureManager&,
                                 NormalMapManager&, TextureManager&,
                                 SpectrumManager&>
    kImpls = {{"matte", ParseMatte}, {"plastic", ParsePlastic}};

}  // namespace

MaterialResult ParseMaterial(Directive& directive,
                             const NamedTextureManager& named_texture_manager,
                             NormalMapManager& normal_map_manager,
                             TextureManager& texture_manager,
                             SpectrumManager& spectrum_manager) {
  return directive.Invoke(kImpls, named_texture_manager, normal_map_manager,
                          texture_manager, spectrum_manager);
}

std::pair<std::string, MaterialResult> ParseMakeNamedMaterial(
    Directive& directive, const NamedTextureManager& named_texture_manager,
    NormalMapManager& normal_map_manager, TextureManager& texture_manager,
    SpectrumManager& spectrum_manager) {
  return directive.InvokeNamedTyped(kImpls, named_texture_manager,
                                    normal_map_manager, texture_manager,
                                    spectrum_manager);
}

std::string ParseNamedMaterial(Directive& directive) {
  return directive.SingleString("name");
}

}  // namespace iris