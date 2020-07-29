#ifndef _SRC_MATERIALS_PLASTIC_
#define _SRC_MATERIALS_PLASTIC_

#include "src/common/material_factory.h"
#include "src/common/named_texture_manager.h"
#include "src/common/normal_map_manager.h"
#include "src/common/spectrum_manager.h"
#include "src/common/texture_manager.h"
#include "src/common/tokenizer.h"
#include "src/param_matchers/parser.h"

namespace iris {

MaterialFactory ParsePlastic(const char* base_type_name, const char* type_name,
                             Tokenizer& tokenizer,
                             const NamedTextureManager& named_texture_manager,
                             NormalMapManager& normal_map_manager,
                             TextureManager& texture_manager,
                             SpectrumManager& spectrum_manager);

MaterialFactory MakeNamedPlastic(
    const char* base_type_name, const char* type_name,
    const Tokenizer& tokenizer, absl::Span<Parameter> parameters,
    const NamedTextureManager& named_texture_manager,
    NormalMapManager& normal_map_manager, TextureManager& texture_manager,
    SpectrumManager& spectrum_manager);

}  // namespace iris

#endif  // _SRC_MATERIALS_PLASTIC_