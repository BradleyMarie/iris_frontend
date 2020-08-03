#include "src/common/directive.h"

#include <algorithm>
#include <iostream>

#include "src/common/quoted_string.h"
#include "src/param_matchers/single.h"

namespace iris {
namespace {

absl::string_view InvalidValue [[noreturn]](absl::string_view base_type_name,
                                            absl::string_view value,
                                            absl::string_view element_name) {
  std::cerr << "ERROR: Invalid " << base_type_name << " " << element_name
            << " specified: " << value << std::endl;
  exit(EXIT_FAILURE);
}

absl::string_view ParseNextQuotedString(absl::string_view base_type_name,
                                        Tokenizer& tokenizer,
                                        absl::string_view element_name) {
  auto token = tokenizer.Next();
  if (!token.has_value()) {
    std::cerr << "ERROR: " << base_type_name << " " << element_name
              << " not specified" << std::endl;
    exit(EXIT_FAILURE);
  }

  auto unquoted = UnquoteToken(*token);
  if (!unquoted.has_value()) {
    InvalidValue(base_type_name, *token, element_name);
  }

  return *unquoted;
}

std::string ParseNextQuotedStringAndCopy(
    absl::string_view base_type_name, Tokenizer& tokenizer,
    absl::string_view element_name) {
  auto result = ParseNextQuotedString(base_type_name, tokenizer, element_name);
  return std::string(result);
}

size_t Match(absl::Span<const absl::string_view> values,
             absl::string_view base_type_name,
             absl::string_view value,
             absl::string_view element_name) {
  auto iter = std::find(values.begin(), values.end(), value);
  if (iter != values.end()) {
    return std::distance(values.begin(), iter);
  }

  InvalidValue(base_type_name, value, element_name);
}

size_t ParseAndMatch(absl::Span<const absl::string_view> type_names,
                     absl::string_view base_type_name,
                     Tokenizer& tokenizer,
                     absl::string_view element_name) {
  auto value = ParseNextQuotedString(base_type_name, tokenizer, element_name);
  return Match(type_names, base_type_name, value, element_name);
}

}  // namespace

Directive::Directive(absl::string_view base_type_name, Tokenizer& tokenizer)
    : m_base_type_name(base_type_name), m_tokenizer(&tokenizer) { }

Directive::~Directive() { assert(!m_tokenizer); }

size_t Directive::MatchType(absl::Span<const absl::string_view> type_names) {
  return ParseAndMatch(type_names, m_base_type_name, *m_tokenizer, "type");
}

size_t Directive::MatchFormat(absl::Span<const absl::string_view> type_names) {
  return ParseAndMatch(type_names, m_base_type_name, *m_tokenizer, "format");
}

std::string Directive::SingleString(absl::string_view field_name) {
  auto result =
      ParseNextQuotedStringAndCopy(m_base_type_name, *m_tokenizer, field_name);
  m_tokenizer = nullptr;
  return result;
}

void Directive::Empty() {
  Parameters parameters(m_base_type_name, *m_tokenizer);
  parameters.Match();
  m_tokenizer = nullptr;
}

void Directive::Ignore() {
  ParseNextQuotedString(m_base_type_name, *m_tokenizer, "type");

  Parameters parameters(m_base_type_name, *m_tokenizer);
  parameters.Ignore();

  std::cerr << "WARNING: Directive " << m_base_type_name
            << " is not implemented and was ignored." << std::endl;

  m_tokenizer = nullptr;
}

std::string Directive::ParseName() {
  return ParseNextQuotedStringAndCopy(m_base_type_name, *m_tokenizer, "name");
}

absl::string_view Directive::ParseFormat() {
  return ParseNextQuotedString(m_base_type_name, *m_tokenizer, "format");
}

std::pair<Parameters, size_t> Directive::MatchTyped(
    absl::Span<const absl::string_view> type_names) {
  SingleStringMatcher type("type", true, std::string());
  Parameters parameters(m_base_type_name, *m_tokenizer);
  auto unused_params = parameters.MatchAllowUnused(type);
  auto index = Match(type_names, m_base_type_name, type.Get(), "type");
  unused_params.m_type_name = type_names[index];
  return std::make_pair(std::move(unused_params), index);
}

}  // namespace iris