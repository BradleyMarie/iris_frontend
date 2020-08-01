#include "src/common/directive.h"

#include <algorithm>
#include <iostream>

#include "src/common/quoted_string.h"
#include "src/param_matchers/single.h"

namespace iris {
namespace {

std::string ParseNextQuotedString(absl::string_view base_type_name,
                                  Tokenizer& tokenizer,
                                  absl::string_view element_name) {
  auto token = tokenizer.Next();
  if (!token) {
    std::cerr << "ERROR: " << base_type_name << " " << element_name
              << " not specified" << std::endl;
    exit(EXIT_FAILURE);
  }

  auto unquoted = UnquoteToken(*token);
  if (!unquoted) {
    std::cerr << "ERROR: Invalid " << base_type_name << " " << element_name
              << " specified: " << *token << std::endl;
    exit(EXIT_FAILURE);
  }

  return std::string(*unquoted);
}

size_t MatchType(absl::Span<const absl::string_view> type_names,
                 absl::string_view base_type_name, absl::string_view token,
                 bool unquote) {
  absl::optional<absl::string_view> opt_token = token;
  if (unquote) {
    opt_token = UnquoteToken(token);
  }
  if (opt_token.has_value()) {
    auto iter = std::find(type_names.begin(), type_names.end(), *opt_token);
    if (iter != type_names.end()) {
      return std::distance(type_names.begin(), iter);
    }
  }

  std::cerr << "ERROR: Invalid " << base_type_name << " specified: " << token
            << std::endl;
  exit(EXIT_FAILURE);
}

}  // namespace

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

  return MatchType(type_names, m_base_type_name, *token, true);
}

void Directive::Ignore() {
  auto quoted_subtype = m_tokenizer->Next();
  if (!quoted_subtype) {
    std::cerr << "ERROR: " << m_base_type_name << " type not specified"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  auto subtype = UnquoteToken(*quoted_subtype);
  if (!subtype) {
    std::cerr << "ERROR: Invalid " << m_base_type_name
              << " specified: " << *subtype << std::endl;
    exit(EXIT_FAILURE);
  }

  Parameters parameters(m_base_type_name, *m_tokenizer);
  parameters.Ignore();

  std::cerr << "WARNING: Directive " << m_base_type_name
            << " is not implemented and was ignored." << std::endl;

  m_tokenizer = nullptr;
}

std::string Directive::SingleString(absl::string_view field_name) {
  auto str = ParseNextQuotedString(m_base_type_name, *m_tokenizer, field_name);
  m_tokenizer = nullptr;
  return str;
}

std::string Directive::ParseName() {
  return ParseNextQuotedString(m_base_type_name, *m_tokenizer, "name");
}

std::pair<Parameters, size_t> Directive::MatchTyped(
    absl::Span<const absl::string_view> type_names) {
  SingleStringMatcher type("type", true, std::string());
  Parameters parameters(m_base_type_name, *m_tokenizer);
  auto unused_params = parameters.MatchAllowUnused(type);
  auto index = MatchType(type_names, m_base_type_name, type.Get(), false);
  unused_params.SetType(type_names[index]);
  return std::make_pair(std::move(unused_params), index);
}

}  // namespace iris