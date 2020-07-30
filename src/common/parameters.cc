#include "src/common/parameters.h"

namespace iris {

namespace {

void MatchParameter(absl::string_view base_type_name,
                    absl::string_view type_name,
                    absl::Span<ParamMatcher* const> param_matchers,
                    Parameter& parameter,
                    std::vector<Parameter>* unhandled_parameters) {
  bool found = false;
  for (auto& current : param_matchers) {
    if (current->Match(base_type_name, type_name, parameter)) {
      found = true;
      break;
    }
  }
  if (!found) {
    if (!unhandled_parameters) {
      std::cerr << "ERROR: Unrecognized or misconfigured parameter to "
                << type_name << " " << base_type_name << ": " << parameter.first
                << std::endl;
      exit(EXIT_FAILURE);
    }

    unhandled_parameters->push_back(parameter);
  }
}

void MatchParametersF(absl::string_view base_type_name,
                      absl::string_view type_name, Tokenizer& tokenizer,
                      absl::Span<ParamMatcher* const> param_matchers,
                      std::vector<Parameter>* unhandled_parameters) {
  for (auto parameter = ParseNextParam(tokenizer); parameter.has_value();
       parameter = ParseNextParam(tokenizer)) {
    MatchParameter(base_type_name, type_name, param_matchers, *parameter,
                   unhandled_parameters);
  }
  for (auto matcher : param_matchers) {
    matcher->Validate(base_type_name, type_name);
  }
}

void MatchParameters(absl::string_view base_type_name,
                     absl::string_view type_name,
                     std::vector<Parameter>& parameters,
                     absl::Span<ParamMatcher* const> param_matchers,
                     std::vector<Parameter>* unhandled_parameters) {
  for (auto& parameter : parameters) {
    MatchParameter(base_type_name, type_name, param_matchers, parameter,
                   unhandled_parameters);
  }
  for (auto matcher : param_matchers) {
    matcher->Validate(base_type_name, type_name);
  }
}

}  // namespace

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

Parameters Parameters::MatchAllowUnusedImpl(
    absl::Span<ParamMatcher* const> param_matchers) {
  assert(m_tokenizer || m_unused_parameters);
  std::vector<Parameter> unused_parameters;
  if (m_tokenizer) {
    MatchParametersF(m_base_type_name, m_type_name, *m_tokenizer,
                     param_matchers, &unused_parameters);
    m_tokenizer = nullptr;
  } else {
    MatchParameters(m_base_type_name, m_type_name, *m_unused_parameters,
                    param_matchers, &unused_parameters);
    m_unused_parameters = absl::nullopt;
  }
  return Parameters(m_base_type_name, m_type_name,
                    std::move(unused_parameters));
}

void Parameters::MatchImpl(absl::Span<ParamMatcher* const> param_matchers) {
  assert(m_tokenizer || m_unused_parameters);
  if (m_tokenizer) {
    MatchParametersF(m_base_type_name, m_type_name, *m_tokenizer,
                     param_matchers, nullptr);
    m_tokenizer = nullptr;
  } else {
    MatchParameters(m_base_type_name, m_type_name, *m_unused_parameters,
                    param_matchers, nullptr);
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