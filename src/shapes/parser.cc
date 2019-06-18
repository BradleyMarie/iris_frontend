#include "src/shapes/parser.h"
#include "src/directive_parser.h"
#include "src/shapes/trianglemesh.h"

#include <iostream>

namespace iris {

ShapeResult ParseShape(
    const char* base_type_name, Tokenizer& tokenizer,
    const absl::optional<Material>& front_material,
    const absl::optional<Material>& back_material,
    const absl::optional<EmissiveMaterial>& front_emissive_material,
    const absl::optional<EmissiveMaterial>& back_emissive_material) {
  return ParseDirective<ShapeResult, 1, const absl::optional<Material>&,
                        const absl::optional<Material>&,
                        const absl::optional<EmissiveMaterial>&,
                        const absl::optional<EmissiveMaterial>&>(
      base_type_name, tokenizer,
      {std::make_pair("trianglemesh", ParseTriangleMesh)}, front_material,
      back_material, front_emissive_material, back_emissive_material);
}

}  // namespace iris