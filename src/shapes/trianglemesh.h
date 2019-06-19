#ifndef _SRC_SHAPES_TRIANGLEMESH_
#define _SRC_SHAPES_TRIANGLEMESH_

#include "src/shapes/result.h"
#include "src/tokenizer.h"

namespace iris {

ShapeResult ParseTriangleMesh(const char* base_type_name, const char* type_name,
                              Tokenizer& tokenizer,
                              const Material& front_material,
                              const Material& back_material,
                              const EmissiveMaterial& front_emissive_material,
                              const EmissiveMaterial& back_emissive_material);

}  // namespace iris

#endif  // _SRC_SHAPES_TRIANGLEMESH_