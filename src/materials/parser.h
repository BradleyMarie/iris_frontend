#ifndef _SRC_MATERIALS_PARSER_
#define _SRC_MATERIALS_PARSER_

#include "src/common/spectrum_manager.h"
#include "src/common/texture_manager.h"
#include "src/common/tokenizer.h"

namespace iris {

Material ParseMaterial(const char* base_type_name, Tokenizer& tokenizer,
                       SpectrumManager& spectrum_manager,
                       TextureManager& texture_manager);

}  // namespace iris

#endif  // _SRC_MATERIALS_PARSER_