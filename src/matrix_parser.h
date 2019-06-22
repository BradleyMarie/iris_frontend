#ifndef _SRC_MATRIX_PARSER_
#define _SRC_MATRIX_PARSER_

#include "src/common/matrix_manager.h"
#include "src/common/tokenizer.h"

namespace iris {

bool TryParseMatrix(absl::string_view directive, Tokenizer& tokenizer,
                    MatrixManager& matrix_manager);

}  // namespace iris

#endif  // _SRC_MATRIX_PARSER_