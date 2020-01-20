#include "src/shapes/parser.h"

#include <iostream>

#include "src/common/call_directive.h"
#include "src/shapes/plymesh.h"
#include "src/shapes/trianglemesh.h"

namespace iris {

ShapeResult ParseShape(const char* base_type_name, Tokenizer& tokenizer,
                       MaterialManager& material_manager,
                       const NamedTextureManager& named_texture_manager,
                       TextureManager& texture_manager,
                       SpectrumManager& spectrum_manager,
                       const MaterialFactory& material_factory,
                       const EmissiveMaterial& front_emissive_material,
                       const EmissiveMaterial& back_emissive_material) {
  return CallDirective<ShapeResult, 2, MaterialManager&,
                       const NamedTextureManager&, TextureManager&,
                       SpectrumManager&, const MaterialFactory&,
                       const EmissiveMaterial&, const EmissiveMaterial&>(
      base_type_name, tokenizer,
      {std::make_pair("trianglemesh", ParseTriangleMesh),
       std::make_pair("plymesh", ParsePlyMesh)},
      material_manager, named_texture_manager, texture_manager,
      spectrum_manager, material_factory, front_emissive_material,
      back_emissive_material);
}

}  // namespace iris