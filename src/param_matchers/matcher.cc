#include "src/param_matchers/matcher.h"

#include <iostream>

#include "absl/strings/str_cat.h"

namespace iris {

std::string ErrorTypeName(absl::string_view base_type_name,
                          absl::optional<absl::string_view> type_name) {
  if (!type_name) {
    return std::string(base_type_name);
  }
  return absl::StrCat(*type_name, " ", base_type_name);
}

bool ParamMatcher::Match(absl::string_view base_type_name,
                         absl::optional<absl::string_view> type_name,
                         Parameter& parameter) {
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
      std::cerr << "ERROR: Redefinition of "
                << ErrorTypeName(m_base_type_name, m_type_name)
                << " parameter: " << m_parameter_name << std::endl;
      exit(EXIT_FAILURE);
    }

    Match(parameter.second);
    m_found = true;

    return true;
  }

  std::cerr << "ERROR: Wrong type for "
            << ErrorTypeName(m_base_type_name, m_type_name)
            << " parameter : " << m_parameter_name << std::endl;
  exit(EXIT_FAILURE);
}

void ParamMatcher::Validate(absl::string_view base_type_name,
                            absl::optional<absl::string_view> type_name) const {
  if (!m_required || m_found) {
    return;
  }

  std::cerr << "ERROR: Missing value for required "
            << ErrorTypeName(m_base_type_name, m_type_name)
            << " parameter: " << m_parameter_name << std::endl;
  exit(EXIT_FAILURE);
}

void ParamMatcher::NumberOfElementsError [[noreturn]] () const {
  std::cerr << "ERROR: Wrong number of values for "
            << ErrorTypeName(m_base_type_name, m_type_name)
            << " parameter: " << m_parameter_name << std::endl;
  exit(EXIT_FAILURE);
}

void ParamMatcher::ElementRangeError [[noreturn]] () const {
  std::cerr << "ERROR: Out of range value for "
            << ErrorTypeName(m_base_type_name, m_type_name)
            << " parameter: " << m_parameter_name << std::endl;
  exit(EXIT_FAILURE);
}

}  // namespace iris