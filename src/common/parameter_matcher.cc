#include "src/common/parameter_matcher.h"

#include <iostream>

#include "absl/strings/str_cat.h"

namespace iris {
namespace {

std::string ErrorTypeName(absl::string_view base_type_name,
                          absl::optional<absl::string_view> type_name) {
  if (!type_name) {
    return std::string(base_type_name);
  }
  return absl::StrCat(*type_name, " ", base_type_name);
}

}  // namespace

ParameterMatcher::ParameterMatcher(absl::string_view parameter_name,
                                   bool required,
                                   absl::Span<const size_t> variant_indices)
    : m_parameter_name(parameter_name),
      m_variant_indices(variant_indices),
      m_required(required),
      m_found(false) {}

bool ParameterMatcher::Match(absl::string_view base_type_name,
                             absl::optional<absl::string_view> type_name,
                             Parameter& parameter) {
  if (parameter.first != m_parameter_name) {
    return false;
  }

  m_base_type_name = base_type_name;
  m_type_name = type_name;

  for (size_t i = 0; i < m_variant_indices.size(); i++) {
    if (parameter.second.index() != m_variant_indices[i]) {
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

void ParameterMatcher::Validate(
    absl::string_view base_type_name,
    absl::optional<absl::string_view> type_name) const {
  if (!m_required || m_found) {
    return;
  }

  std::cerr << "ERROR: Missing value for required "
            << ErrorTypeName(m_base_type_name, m_type_name)
            << " parameter: " << m_parameter_name << std::endl;
  exit(EXIT_FAILURE);
}

void ParameterMatcher::NumberOfElementsError [[noreturn]] () const {
  std::cerr << "ERROR: Wrong number of values for "
            << ErrorTypeName(m_base_type_name, m_type_name)
            << " parameter: " << m_parameter_name << std::endl;
  exit(EXIT_FAILURE);
}

void ParameterMatcher::ElementRangeError [[noreturn]] () const {
  std::cerr << "ERROR: Out of range value for "
            << ErrorTypeName(m_base_type_name, m_type_name)
            << " parameter: " << m_parameter_name << std::endl;
  exit(EXIT_FAILURE);
}

}  // namespace iris