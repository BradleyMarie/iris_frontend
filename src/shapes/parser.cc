#include "src/shapes/parser.h"

#include "src/common/call_directive.h"
#include "src/shapes/plymesh.h"
#include "src/shapes/sphere.h"
#include "src/shapes/trianglemesh.h"

namespace iris {

ShapeResult ParseShape(absl::string_view base_type_name, Tokenizer& tokenizer,
                       const Matrix& model_to_world,
                       MaterialManager& material_manager,
                       const NamedTextureManager& named_texture_manager,
                       NormalMapManager& normal_map_manager,
                       TextureManager& texture_manager,
                       SpectrumManager& spectrum_manager,
                       const MaterialResult& material,
                       const EmissiveMaterial& front_emissive_material,
                       const EmissiveMaterial& back_emissive_material) {
  return CallDirective<
      ShapeResult, const Matrix&, MaterialManager&, const NamedTextureManager&,
      NormalMapManager&, TextureManager&, SpectrumManager&,
      const MaterialResult&, const EmissiveMaterial&, const EmissiveMaterial&>(
      base_type_name, tokenizer,
      {{"plymesh", ParsePlyMesh},
       {"sphere", ParseSphere},
       {"trianglemesh", ParseTriangleMesh}},
      model_to_world, material_manager, named_texture_manager,
      normal_map_manager, texture_manager, spectrum_manager, material,
      front_emissive_material, back_emissive_material);
}

}  // namespace iris