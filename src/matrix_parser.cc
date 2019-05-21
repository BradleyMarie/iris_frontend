#include <iostream>

#include "src/matrix_parser.h"

namespace iris {

bool TryParseMatrix(absl::string_view directive, Tokenizer& tokenizer,
                    MatrixManager& matrix_manager) {
  std::cerr << "ERROR: TryParseMatrix not implemented" << std::endl;
  exit(EXIT_FAILURE);
}

}  // namespace iris