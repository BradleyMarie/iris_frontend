#ifndef _SRC_COMMON_CALL_DIRECTIVE_
#define _SRC_COMMON_CALL_DIRECTIVE_

#include <array>
#include <functional>
#include <iostream>
#include <utility>

#include "src/common/matrix_manager.h"
#include "src/common/parameters.h"
#include "src/common/quoted_string.h"
#include "src/common/tokenizer.h"

namespace iris {

template <typename Result, typename... Args>
using DirectiveImpl =
    std::function<Result(const char* base_type_name, const char* type_name,
                         Tokenizer&, Args... args)>;

template <typename Result, size_t NumImplementations, typename... Args>
Result CallDirective(
    const char* base_type_name, Tokenizer& tokenizer,
    const std::array<std::pair<const char*, DirectiveImpl<Result, Args...>>,
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

template <typename Result, typename... Args>
using ParameterDirectiveImpl =
    std::function<Result(const char* base_type_name, const char* type_name,
                         Parameters&, Args... args)>;

template <typename Result, size_t NumImplementations, typename... Args>
Result CallDirective(
    const char* base_type_name, Tokenizer& tokenizer,
    const std::array<
        std::pair<const char*, ParameterDirectiveImpl<Result, Args...>>,
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
        Parameters parameters(base_type_name, entry.first, tokenizer);
        return entry.second(base_type_name, entry.first, parameters, args...);
      }
    }
  }

  std::cerr << "ERROR: Invalid " << base_type_name << " specified: " << *token
            << std::endl;
  exit(EXIT_FAILURE);
}

}  // namespace iris

#endif  // _SRC_COMMON_CALL_DIRECTIVE_