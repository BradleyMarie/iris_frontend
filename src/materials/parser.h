#ifndef _SRC_MATERIALS_PARSER_
#define _SRC_MATERIALS_PARSER_

#include "src/materials/result.h"
#include "src/tokenizer.h"

namespace iris {

MaterialResult ParseMaterial(const char* base_type_name, Tokenizer& tokenizer);

}  // namespace iris

#endif  // _SRC_MATERIALS_PARSER_