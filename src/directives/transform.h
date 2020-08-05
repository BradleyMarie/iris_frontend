#ifndef _SRC_DIRECTIVES_TRANSFORM_
#define _SRC_DIRECTIVES_TRANSFORM_

#include "src/common/tokenizer.h"
#include "src/directives/matrix_manager.h"

namespace iris {

bool TryParseTransformDirectives(absl::string_view directive,
                                 Tokenizer& tokenizer,
                                 MatrixManager& matrix_manager);

}  // namespace iris

#endif  // _SRC_DIRECTIVES_TRANSFORM_