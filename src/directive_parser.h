#ifndef _SRC_DIRECTIVE_PARSER_
#define _SRC_DIRECTIVE_PARSER_

#include <array>
#include <functional>
#include <iostream>
#include <utility>

#include "src/common/matrix_manager.h"
#include "src/common/tokenizer.h"
#include "src/quoted_string.h"

namespace iris {

template <typename Result, typename... Args>
using DirectiveCallback =
    std::function<Result(const char* base_type_name, const char* type_name,
                         Tokenizer&, Args... args)>;

template <typename Result, size_t NumImplementations, typename... Args>
Result ParseDirective(
    const char* base_type_name, Tokenizer& tokenizer,
    const std::array<std::pair<const char*, DirectiveCallback<Result, Args...>>,
                     NumImplementations>& callbacks,
    Args... args) {
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
        return entry.second(base_type_name, entry.first, tokenizer, args...);
      }
    }
  }

  std::cerr << "ERROR: Invalid " << base_type_name << " specified: " << *token
            << std::endl;
  exit(EXIT_FAILURE);
}

}  // namespace iris

#endif  // _SRC_DIRECTIVE_PARSER_