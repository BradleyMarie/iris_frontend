#ifndef _SRC_MATERIALS_MATTE_
#define _SRC_MATERIALS_MATTE_

#include "src/materials/result.h"
#include "src/tokenizer.h"

namespace iris {

MaterialResult ParseMatte(const char* base_type_name, const char* type_name,
                          Tokenizer& tokenizer);

}  // namespace iris

#endif  // _SRC_MATERIALS_MATTE_