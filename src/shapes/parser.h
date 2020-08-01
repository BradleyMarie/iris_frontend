#ifndef _SRC_SHAPES_PARSER_
#define _SRC_SHAPES_PARSER_

#include "src/common/directive.h"
#include "src/materials/result.h"
#include "src/shapes/result.h"

namespace iris {

ShapeResult ParseShape(Directive& directive, const Matrix& model_to_world,
                       MaterialManager& material_manager,
                       const NamedTextureManager& named_texture_manager,
                       NormalMapManager& normal_map_manager,
                       TextureManager& texture_manager,
                       SpectrumManager& spectrum_manager,
                       const MaterialResult& material,
                       const EmissiveMaterial& front_emissive_material,
                       const EmissiveMaterial& back_emissive_material);

}  // namespace iris

#endif  // _SRC_SHAPES_PARSER_