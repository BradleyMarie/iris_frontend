#ifndef _SRC_TEXTURES_CONSTANT_
#define _SRC_TEXTURES_CONSTANT_

#include "src/common/spectrum_manager.h"
#include "src/common/texture_manager.h"
#include "src/common/tokenizer.h"

namespace iris {

ReflectorTexture ParseConstantReflector(const char* base_type_name,
                                        const char* type_name,
                                        Tokenizer& tokenizer,
                                        SpectrumManager& spectrum_manager,
                                        const TextureManager& texture_manager);

FloatTexture ParseConstantFloat(const char* base_type_name,
                                const char* type_name, Tokenizer& tokenizer,
                                const TextureManager& texture_manager);

}  // namespace iris

#endif  // _SRC_TEXTURES_CONSTANT_