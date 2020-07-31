#ifndef _SRC_RANDOMS_PARSER_
#define _SRC_RANDOMS_PARSER_

#include "src/common/pointer_types.h"
#include "src/common/tokenizer.h"

namespace iris {

Random ParseRandom(absl::string_view base_type_name, Tokenizer& tokenizer);

Random CreateDefaultRandom();

}  // namespace iris

#endif  // _SRC_RANDOMS_PARSER_