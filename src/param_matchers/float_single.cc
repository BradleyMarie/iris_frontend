#include "src/param_matchers/float_single.h"

namespace iris {

const size_t SingleFloatMatcher::m_variant_type =
    GetIndex<FloatParameter, ParameterData>();

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