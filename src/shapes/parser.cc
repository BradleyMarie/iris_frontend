#include "src/shapes/parser.h"

#include <iostream>

namespace iris {

ShapeResult ParseShape(
    Tokenizer& tokenizer, const absl::optional<Material>& front_material,
    const absl::optional<Material>& back_material,
    const absl::optional<EmissiveMaterial>& front_emissive_material,
    const absl::optional<EmissiveMaterial>& back_emissive_material) {
  std::cerr << "ERROR: ParseShape is not implemented" << std::endl;
  exit(EXIT_FAILURE);
}

}  // namespace iris