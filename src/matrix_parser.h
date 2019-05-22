#ifndef _SRC_MATRIX_PARSER_
#define _SRC_MATRIX_PARSER_

#include "src/matrix_manager.h"
#include "src/tokenizer.h"

namespace iris {

bool TryParseMatrix(absl::string_view directive, Tokenizer& tokenizer,
                    MatrixManager& matrix_manager);

}  // namespace iris

#endif  // _SRC_MATRIX_PARSER_