#include "src/param_matchers/matcher.h"

#include <iostream>

namespace iris {

bool ParamMatcher::Match(absl::string_view base_type_name,
                         absl::string_view type_name, Parameter& parameter) {
  if (parameter.first != m_parameter_name) {
    return false;
  }

  m_base_type_name = base_type_name;
  m_type_name = type_name;

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

void ParamMatcher::Validate(absl::string_view base_type_name,
                            absl::string_view type_name) const {
  if (!m_required || m_found) {
    return;
  }

  std::cerr << "ERROR: Missing value for required " << type_name << " "
            << base_type_name << " parameter: " << m_parameter_name
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

}  // namespace iris