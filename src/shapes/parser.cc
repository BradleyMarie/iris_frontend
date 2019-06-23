#include "src/shapes/parser.h"

#include <iostream>

#include "src/common/call_directive.h"
#include "src/shapes/trianglemesh.h"

namespace iris {

ShapeResult ParseShape(const char* base_type_name, Tokenizer& tokenizer,
                       const Material& front_material,
                       const Material& back_material,
                       const EmissiveMaterial& front_emissive_material,
                       const EmissiveMaterial& back_emissive_material) {
  return CallDirective<ShapeResult, 1, const Material&, const Material&,
                        const EmissiveMaterial&, const EmissiveMaterial&>(
      base_type_name, tokenizer,
      {std::make_pair("trianglemesh", ParseTriangleMesh)}, front_material,
      back_material, front_emissive_material, back_emissive_material);
}

}  // namespace iris