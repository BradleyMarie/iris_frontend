#include "src/param_matchers/float_single.h"

namespace iris {
namespace {

const size_t variant_indices[1] = {GetIndex<FloatParameter, ParameterData>()};

}  // namespace

SingleFloatMatcher::SingleFloatMatcher(absl::string_view parameter_name,
                                       bool required, bool inclusive,
                                       float_t minimum, float_t maximum,
                                       absl::optional<float_t> default_value)
    : ParameterMatcher(parameter_name, required, variant_indices),
      m_minimum(minimum),
      m_maximum(maximum),
      m_value(default_value),
      m_inclusive(inclusive) {
  assert(!std::isnan(minimum));
  assert(!std::isnan(maximum));
  assert((inclusive && minimum <= maximum) ||
         (!inclusive && minimum < maximum));
  if (default_value) {
    assert(ValidateFloat(*default_value));
  }
}

const absl::optional<float_t>& SingleFloatMatcher::Get() const {
  return m_value;
}

bool SingleFloatMatcher::ValidateFloat(float_t value) const {
  bool valid;
  if (m_inclusive) {
    valid = m_minimum <= value && value <= m_maximum;
  } else {
    valid = m_minimum < value && value < m_maximum;
  }

  return valid;
}

void SingleFloatMatcher::Match(ParameterData& data) {
  if (absl::get<FloatParameter>(data).data.size() != 1) {
    NumberOfElementsError();
  }
  auto value = absl::get<FloatParameter>(data).data[0];
  if (!ValidateFloat(value)) {
    ElementRangeError();
  }
  m_value = static_cast<float_t>(absl::get<FloatParameter>(data).data[0]);
}

}  // namespace iris