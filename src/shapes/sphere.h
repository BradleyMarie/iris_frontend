#ifndef _SRC_SHAPES_SPHERE_
#define _SRC_SHAPES_SPHERE_

#include "src/common/material_factory.h"
#include "src/common/tokenizer.h"
#include "src/shapes/result.h"

namespace iris {

ShapeResult ParseSphere(const char* base_type_name, const char* type_name,
                        Tokenizer& tokenizer, MaterialManager& material_manager,
                        const NamedTextureManager& named_texture_manager,
                        NormalMapManager& normal_map_manager,
                        TextureManager& texture_manager,
                        SpectrumManager& spectrum_manager,
                        const MaterialFactory& material_factory,
                        const EmissiveMaterial& front_emissive_material,
                        const EmissiveMaterial& back_emissive_material);

}  // namespace iris

#endif  // _SRC_SHAPES_SPHERE_