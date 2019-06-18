#include "src/shapes/parser.h"
#include "src/directive_parser.h"
#include "src/shapes/trianglemesh.h"

#include <iostream>

namespace iris {

ShapeResult ParseShape(const char* base_type_name, Tokenizer& tokenizer,
                       const Material& front_material,
                       const Material& back_material,
                       const EmissiveMaterial& front_emissive_material,
                       const EmissiveMaterial& back_emissive_material) {
  return ParseDirective<ShapeResult, 1, const Material&, const Material&,
                        const EmissiveMaterial&, const EmissiveMaterial&>(
      base_type_name, tokenizer,
      {std::make_pair("trianglemesh", ParseTriangleMesh)}, front_material,
      back_material, front_emissive_material, back_emissive_material);
}

}  // namespace iris