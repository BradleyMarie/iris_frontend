#ifndef _SRC_MATERIALS_PARSER_
#define _SRC_MATERIALS_PARSER_

#include "src/common/spectrum_manager.h"
#include "src/common/texture_manager.h"
#include "src/common/tokenizer.h"
#include "src/materials/result.h"

namespace iris {

MaterialResult ParseMaterial(const char* base_type_name, Tokenizer& tokenizer,
                             SpectrumManager& spectrum_manager,
                             const TextureManager& texture_manager);

}  // namespace iris

#endif  // _SRC_MATERIALS_PARSER_