#ifndef _SRC_MATERIALS_MATTE_
#define _SRC_MATERIALS_MATTE_

#include "src/common/material_manager.h"
#include "src/common/named_texture_manager.h"
#include "src/common/spectrum_manager.h"
#include "src/common/texture_manager.h"
#include "src/common/tokenizer.h"
#include "src/param_matchers/parser.h"

namespace iris {

Material ParseMatte(const char* base_type_name, const char* type_name,
                    Tokenizer& tokenizer, MaterialManager& material_manager,
                    const NamedTextureManager& named_texture_manager,
                    TextureManager& texture_manager,
                    SpectrumManager& spectrum_manager);

Material MakeNamedMatte(const char* base_type_name, const char* type_name,
                        std::vector<Parameter>& parameters,
                        MaterialManager& material_manager,
                        const NamedTextureManager& named_texture_manager,
                        TextureManager& texture_manager,
                        SpectrumManager& spectrum_manager);

}  // namespace iris

#endif  // _SRC_MATERIALS_MATTE_