#include "src/directives/include.h"

#include <iostream>

#include "src/common/quoted_string.h"

namespace iris {

bool TryParseInclude(absl::string_view directive, Tokenizer& tokenizer) {
  if (directive != "Include") {
    return false;
  }

  auto token = tokenizer.Next();
  if (!token) {
    std::cerr << "ERROR: Include requires 1 parameter" << std::endl;
    exit(EXIT_FAILURE);
  }

  auto unquoted = UnquoteToken(*token);
  if (!unquoted) {
    std::cerr << "ERROR: Failed to parse Include parameter: " << *token
              << std::endl;
    exit(EXIT_FAILURE);
  }

  std::string as_string(unquoted->data(), unquoted->size());
  tokenizer.Include(as_string);

  return true;
}

}  // namespace iris