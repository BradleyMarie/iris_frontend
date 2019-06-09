#ifndef _SRC_DIRECTIVE_PARSER_
#define _SRC_DIRECTIVE_PARSER_

#include <array>
#include <functional>
#include <iostream>
#include <utility>

#include "src/matrix_manager.h"
#include "src/quoted_string.h"
#include "src/tokenizer.h"

namespace iris {

template <typename Result>
using DirectiveCallback =
    std::function<Result(const char* base_type_name, const char* type_name,
                         Tokenizer&, MatrixManager&)>;

template <typename Result, size_t NumImplementations>
Result ParseDirective(
    const char* base_type_name, Tokenizer& tokenizer,
    MatrixManager& matrix_manager,
    const std::array<std::pair<const char*, DirectiveCallback<Result>>,
                     NumImplementations>& callbacks) {
  auto token = tokenizer.Next();
  if (!token) {
    std::cerr << "ERROR: " << base_type_name << " type not specified"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  auto unquoted = UnquoteToken(*token);
  if (unquoted) {
    for (auto& entry : callbacks) {
      if (*unquoted == entry.first) {
        return entry.second(base_type_name, entry.first, tokenizer,
                            matrix_manager);
      }
    }
  }

  std::cerr << "ERROR: Invalid " << base_type_name << " specified: " << *token
            << std::endl;
  exit(EXIT_FAILURE);
}

template <typename Result, size_t NumImplementations>
Result ParseDirectiveOnce(
    const char* base_type_name, Tokenizer& tokenizer,
    MatrixManager& matrix_manager,
    const std::array<std::pair<const char*, DirectiveCallback<Result>>,
                     NumImplementations>& callbacks,
    bool already_set) {
  if (already_set) {
    std::cerr << "ERROR: Invalid " << base_type_name
              << " specified more than once before WorldBegin" << std::endl;
    exit(EXIT_FAILURE);
  }

  return ParseDirective<Result, NumImplementations>(base_type_name, tokenizer,
                                                    matrix_manager, callbacks);
}

}  // namespace iris

#endif  // _SRC_DIRECTIVE_PARSER_