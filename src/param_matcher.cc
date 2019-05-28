#include "src/param_matcher.h"

#include <iostream>

namespace iris {

bool ParamMatcher::Match(const Parameter& parameter) {
  if (parameter.first != m_parameter_name) {
    return false;
  }

  if (parameter.second.index() != m_index) {
    std::cerr << "ERROR: Wrong type for " << m_type_name
              << " parameter : " << m_parameter_name << std::endl;
    exit(EXIT_FAILURE);
  }

  if (m_found) {
    std::cerr << "ERROR: Redefinition of " << m_type_name
              << " parameter: " << m_parameter_name << std::endl;
    exit(EXIT_FAILURE);
  }

  Match(parameter.second);

  return true;
}

void ParamMatcher::NumberOfElementsError[[noreturn]]() {
  std::cerr << "ERROR: Wrong number of values for " << m_type_name
            << " parameter: " << m_parameter_name << std::endl;
  exit(EXIT_FAILURE);
}

void ParamMatcher::ElementRangeError[[noreturn]]() {
  std::cerr << "ERROR: Out of range value for " << m_type_name
            << " parameter: " << m_parameter_name << std::endl;
  exit(EXIT_FAILURE);
}

}  // namespace iris