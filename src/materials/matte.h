#ifndef _SRC_MATERIALS_MATTE_
#define _SRC_MATERIALS_MATTE_

#include "src/common/named_texture_manager.h"
#include "src/common/spectrum_manager.h"
#include "src/common/texture_manager.h"
#include "src/common/tokenizer.h"

namespace iris {

Material ParseMatte(const char* base_type_name, const char* type_name,
                    Tokenizer& tokenizer,
                    const NamedTextureManager& named_texture_manager,
                    TextureManager& texture_manager,
                    SpectrumManager& spectrum_manager);

}  // namespace iris

#endif  // _SRC_MATERIALS_MATTE_