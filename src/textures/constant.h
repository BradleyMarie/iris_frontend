#ifndef _SRC_TEXTURES_CONSTANT_
#define _SRC_TEXTURES_CONSTANT_

#include "src/common/texture_manager.h"
#include "src/common/tokenizer.h"

namespace iris {

std::pair<ReflectorTexture, std::set<Reflector>> ParseConstantReflector(
    const char* base_type_name, const char* type_name, Tokenizer& tokenizer,
    const TextureManager& texture_manager);

FloatTexture ParseConstantFloat(const char* base_type_name,
                                const char* type_name, Tokenizer& tokenizer,
                                const TextureManager& texture_manager);

}  // namespace iris

#endif  // _SRC_TEXTURES_CONSTANT_