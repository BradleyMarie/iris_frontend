#ifndef _SRC_RANDOMS_PARSER_
#define _SRC_RANDOMS_PARSER_

#include "src/pointer_types.h"
#include "src/tokenizer.h"

namespace iris {

Random ParseRandom(const char* base_type_name, Tokenizer& tokenizer);

Random CreateDefaultRandom();

}  // namespace iris

#endif  // _SRC_RANDOMS_PARSER_