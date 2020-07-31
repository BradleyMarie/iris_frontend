#ifndef _SRC_SHAPES_PLYMESH_
#define _SRC_SHAPES_PLYMESH_

#include "src/common/material_factory.h"
#include "src/common/parameters.h"
#include "src/shapes/result.h"

namespace iris {

ShapeResult ParsePlyMesh(Parameters& parameters, const Matrix& model_to_world,
                         MaterialManager& material_manager,
                         const NamedTextureManager& named_texture_manager,
                         NormalMapManager& normal_map_manager,
                         TextureManager& texture_manager,
                         SpectrumManager& spectrum_manager,
                         const MaterialFactory& material_factory,
                         const EmissiveMaterial& front_emissive_material,
                         const EmissiveMaterial& back_emissive_material);

}  // namespace iris

#endif  // _SRC_SHAPES_PLYMESH_