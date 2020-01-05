#ifndef _SRC_DIRECTIVES_INCLUDE_
#define _SRC_DIRECTIVES_INCLUDE_

#include "src/common/tokenizer.h"

namespace iris {

bool TryParseInclude(absl::string_view directive, Tokenizer& tokenizer);

}  // namespace iris

#endif  // _SRC_DIRECTIVES_INCLUDE_