#ifndef _SRC_SHAPES_PARSER_
#define _SRC_SHAPES_PARSER_

#include "src/shapes/result.h"
#include "src/tokenizer.h"

namespace iris {

ShapeResult ParseShape(
    const char* base_type_name, Tokenizer& tokenizer,
    const absl::optional<Material>& front_material,
    const absl::optional<Material>& back_material,
    const absl::optional<EmissiveMaterial>& front_emissive_material,
    const absl::optional<EmissiveMaterial>& back_emissive_material);

}  // namespace iris

#endif  // _SRC_SHAPES_PARSER_