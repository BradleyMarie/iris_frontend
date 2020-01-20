#include "src/param_matchers/matcher.h"

#include <iostream>

namespace iris {

bool ParamMatcher::Match(Parameter& parameter) {
  if (parameter.first != m_parameter_name) {
    return false;
  }

  for (size_t i = 0; i < m_num_indices; i++) {
    if (parameter.second.index() != m_indices[i]) {
      continue;
    }

    if (m_found) {
      std::cerr << "ERROR: Redefinition of " << m_type_name << " "
                << m_base_type_name << " parameter: " << m_parameter_name
                << std::endl;
      exit(EXIT_FAILURE);
    }

    Match(parameter.second);

    return true;
  }

  std::cerr << "ERROR: Wrong type for " << m_type_name << " "
            << m_base_type_name << " parameter : " << m_parameter_name
            << std::endl;
  exit(EXIT_FAILURE);
}

void ParamMatcher::Validate() const {
  if (!m_required || m_found) {
    return;
  }

  std::cerr << "ERROR: Missing value for required " << m_type_name << " "
            << m_base_type_name << " parameter: " << m_parameter_name
            << std::endl;
  exit(EXIT_FAILURE);
}

void ParamMatcher::NumberOfElementsError [[noreturn]] () const {
  std::cerr << "ERROR: Wrong number of values for " << m_type_name << " "
            << m_base_type_name << " parameter: " << m_parameter_name
            << std::endl;
  exit(EXIT_FAILURE);
}

void ParamMatcher::ElementRangeError [[noreturn]] () const {
  std::cerr << "ERROR: Out of range value for " << m_type_name << " "
            << m_base_type_name << " parameter: " << m_parameter_name
            << std::endl;
  exit(EXIT_FAILURE);
}

void MatchParameters(const char* base_type_name, const char* type_name,
                     Tokenizer& tokenizer,
                     std::vector<ParamMatcher*>& supported_parameters) {
  MatchParameters<typename std::vector<ParamMatcher*>::iterator>(
      base_type_name, type_name, tokenizer, supported_parameters.begin(),
      supported_parameters.end());
}

}  // namespace iris