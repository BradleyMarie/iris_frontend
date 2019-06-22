#ifndef _SRC_PARAM_MATCHER_FLOAT_SINGLE_
#define _SRC_PARAM_MATCHER_FLOAT_SINGLE_

#include <limits>

#include "src/param_matchers/matcher.h"

namespace iris {

class SingleFloatMatcher : public ParamMatcher {
 public:
  SingleFloatMatcher(const char* base_type_name, const char* type_name,
                     const char* parameter_name, bool required, bool inclusive,
                     float_t minimum, float_t maximum, float_t default_value)
      : ParamMatcher(base_type_name, type_name, parameter_name, required,
                     &m_variant_type, 1),
        m_minimum(minimum),
        m_maximum(maximum),
        m_value(default_value),
        m_inclusive(inclusive) {
    assert(std::isfinite(minimum));
    assert(std::isfinite(maximum));
    assert(inclusive || minimum < maximum);
  }
  const float_t& Get() { return m_value; }

 protected:
  void Match(ParameterData& data) final {
    if (absl::get<FloatParameter>(data).data.size() != 1) {
      NumberOfElementsError();
    }
    auto value = absl::get<FloatParameter>(data).data[0];
    if (!std::isfinite(value) || value < m_minimum || m_maximum < value) {
      ElementRangeError();
    }
    m_value = static_cast<float_t>(absl::get<FloatParameter>(data).data[0]);
  }

 private:
  static const size_t m_variant_type;
  float_t m_minimum;
  float_t m_maximum;
  float_t m_value;
  bool m_inclusive;
};

}  // namespace iris

#endif  // _SRC_PARAM_MATCHER_FLOAT_SINGLE_