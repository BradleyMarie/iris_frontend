#include "src/common/directive.h"

#include <algorithm>
#include <iostream>

#include "src/common/quoted_string.h"
#include "src/param_matchers/single.h"

namespace iris {
namespace {

absl::string_view ParseNextQuotedString(absl::string_view base_type_name,
                                        absl::optional<absl::string_view> token,
                                        absl::string_view element_name) {
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

  return *unquoted;
}

std::string ParseNextQuotedStringAndCopy(
    absl::string_view base_type_name, absl::optional<absl::string_view> token,
    absl::string_view element_name) {
  auto result = ParseNextQuotedString(base_type_name, token, element_name);
  return std::string(result);
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
    : m_base_type_name(base_type_name), m_tokenizer(&tokenizer) {
  m_first_token = m_tokenizer->Next();
}

Directive::~Directive() { assert(!m_tokenizer); }

size_t Directive::Match(absl::Span<const absl::string_view> type_names) {
  if (!m_first_token) {
    std::cerr << "ERROR: " << m_base_type_name << " type not specified"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  return MatchType(type_names, m_base_type_name, *m_first_token, true);
}

void Directive::Ignore() {
  if (!m_first_token) {
    std::cerr << "ERROR: " << m_base_type_name << " type not specified"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  auto subtype = UnquoteToken(*m_first_token);
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

void Directive::FormatError [[noreturn]] () {
  auto format = m_tokenizer->Next();
  auto format_name = ParseNextQuotedString(m_base_type_name, format, "format");
  std::cerr << "ERROR: Invalid " << m_base_type_name
            << " format specified: " << format_name << std::endl;
  exit(EXIT_FAILURE);
}

std::string Directive::SingleString(absl::string_view field_name) {
  auto result =
      ParseNextQuotedStringAndCopy(m_base_type_name, m_first_token, field_name);
  m_tokenizer = nullptr;
  return result;
}

std::string Directive::ParseName() {
  if (m_name.has_value()) {
    return std::move(*m_name);
  }
  return ParseNextQuotedStringAndCopy(m_base_type_name, m_first_token, "name");
}

bool Directive::MatchesFormat(absl::string_view type_name) {
  auto format = m_tokenizer->Peek();
  auto unquoted = ParseNextQuotedString(m_base_type_name, format, "format");
  if (type_name != unquoted) {
    return false;
  }
  m_name.emplace(*m_first_token);
  m_tokenizer->Next();
  m_first_token = absl::nullopt;
  return true;
}

std::pair<Parameters, size_t> Directive::MatchTyped(
    absl::Span<const absl::string_view> type_names) {
  SingleStringMatcher type("type", true, std::string());
  Parameters parameters(m_base_type_name, *m_tokenizer);
  auto unused_params = parameters.MatchAllowUnused(type);
  auto index = MatchType(type_names, m_base_type_name, type.Get(), false);
  unused_params.m_type_name = type_names[index];
  return std::make_pair(std::move(unused_params), index);
}

}  // namespace iris