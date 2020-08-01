#include "src/common/directive.h"

#include <algorithm>
#include <iostream>

#include "src/common/quoted_string.h"

namespace iris {

Directive::Directive(absl::string_view base_type_name, Tokenizer& tokenizer)
    : m_base_type_name(base_type_name), m_tokenizer(&tokenizer) {}

Directive::~Directive() { assert(!m_tokenizer); }

size_t Directive::Match(absl::Span<const absl::string_view> type_names) {
  auto token = m_tokenizer->Next();
  if (!token) {
    std::cerr << "ERROR: " << m_base_type_name << " type not specified"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  auto unquoted = UnquoteToken(*token);
  if (unquoted) {
    auto iter = std::find(type_names.begin(), type_names.end(), *unquoted);
    if (iter != type_names.end()) {
        return std::distance(type_names.begin(), iter);
    }
  }

  std::cerr << "ERROR: Invalid " << m_base_type_name << " specified: " << *token
            << std::endl;
  exit(EXIT_FAILURE);
}

}  // namespace iris