#ifndef _SRC_SHAPES_SPHERE_
#define _SRC_SHAPES_SPHERE_

#include "src/common/parameters.h"
#include "src/materials/result.h"
#include "src/shapes/result.h"

namespace iris {

ShapeResult ParseSphere(Parameters& parameters, const Matrix& model_to_world,
                        MaterialManager& material_manager,
                        const NamedTextureManager& named_texture_manager,
                        NormalMapManager& normal_map_manager,
                        TextureManager& texture_manager,
                        SpectrumManager& spectrum_manager,
                        const MaterialResult& material_result,
                        const EmissiveMaterial& front_emissive_material,
                        const EmissiveMaterial& back_emissive_material);

}  // namespace iris

#endif  // _SRC_SHAPES_SPHERE_