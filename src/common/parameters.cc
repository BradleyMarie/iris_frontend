#include "src/common/parameters.h"

namespace iris {

Parameters::Parameters(absl::string_view base_type_name,
                       absl::string_view type_name, Tokenizer& tokenizer)
    : m_base_type_name(base_type_name),
      m_type_name(type_name),
      m_tokenizer(&tokenizer) {}

Parameters::Parameters(absl::string_view base_type_name,
                       absl::string_view type_name,
                       std::vector<Parameter> unused_parameters)
    : m_base_type_name(base_type_name),
      m_type_name(type_name),
      m_unused_parameters(std::move(unused_parameters)) {}

Parameters::Parameters(Parameters&& other)
    : m_base_type_name(other.m_base_type_name),
      m_type_name(other.m_type_name),
      m_unused_parameters(std::move(other.m_unused_parameters)),
      m_tokenizer(other.m_tokenizer) {
  other.m_tokenizer = nullptr;
}

Parameters& Parameters::operator=(Parameters&& other) {
  m_base_type_name = other.m_base_type_name;
  m_type_name = other.m_type_name;
  m_unused_parameters = std::move(other.m_unused_parameters);
  m_tokenizer = other.m_tokenizer;
  other.m_tokenizer = nullptr;
  return *this;
}

Parameters::~Parameters() { assert(!m_unused_parameters && !m_tokenizer); }

Parameters Parameters::MatchAllowUnused(
    absl::Span<ParamMatcher* const> param_matchers) {
  assert(m_tokenizer || m_unused_parameters);
  std::vector<Parameter> unused_parameters;
  if (m_tokenizer) {
    MatchParameters(m_base_type_name, m_type_name, *m_tokenizer, param_matchers,
                    &unused_parameters);
    m_tokenizer = nullptr;
  } else {
    MatchParameters(m_base_type_name, m_type_name,
                    absl::MakeSpan(*m_unused_parameters), param_matchers,
                    &unused_parameters);
    m_unused_parameters = absl::nullopt;
  }
  return Parameters(m_base_type_name, m_type_name,
                    std::move(unused_parameters));
}

void Parameters::Match(absl::Span<ParamMatcher* const> param_matchers) {
  assert(m_tokenizer || m_unused_parameters);
  if (m_tokenizer) {
    MatchParameters(m_base_type_name, m_type_name, *m_tokenizer,
                    param_matchers);
    m_tokenizer = nullptr;
  } else {
    MatchParameters(m_base_type_name, m_type_name,
                    absl::MakeSpan(*m_unused_parameters), param_matchers);
    m_unused_parameters = absl::nullopt;
  }
}

void Parameters::Ignore() {
  assert(m_tokenizer || m_unused_parameters);
  if (m_tokenizer) {
    while (ParseNextParam(*m_tokenizer)) {
      // Do Nothing
    }
    m_tokenizer = nullptr;
  } else {
    m_unused_parameters = absl::nullopt;
  }
}

}  // namespace iris