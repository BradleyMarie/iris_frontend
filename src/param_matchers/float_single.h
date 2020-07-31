#ifndef _SRC_PARAM_MATCHER_FLOAT_SINGLE_
#define _SRC_PARAM_MATCHER_FLOAT_SINGLE_

#include <limits>

#include "absl/types/optional.h"
#include "src/common/parameter_matcher.h"

namespace iris {

class SingleFloatMatcher : public ParamMatcher {
 public:
  SingleFloatMatcher(absl::string_view parameter_name, bool required,
                     bool inclusive, float_t minimum, float_t maximum,
                     absl::optional<float_t> default_value)
      : ParamMatcher(parameter_name, required, &m_variant_type, 1),
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
  const absl::optional<float_t>& Get() { return m_value; }

 protected:
  void Match(ParameterData& data) final;

 private:
  bool ValidateFloat(float_t value) const;

  static const size_t m_variant_type;
  float_t m_minimum;
  float_t m_maximum;
  absl::optional<float_t> m_value;
  bool m_inclusive;
};

}  // namespace iris

#endif  // _SRC_PARAM_MATCHER_FLOAT_SINGLE_