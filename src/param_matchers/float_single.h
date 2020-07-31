#ifndef _SRC_PARAM_MATCHER_FLOAT_SINGLE_
#define _SRC_PARAM_MATCHER_FLOAT_SINGLE_

#include "src/common/parameter_matcher.h"

namespace iris {

class SingleFloatMatcher : public ParameterMatcher {
 public:
  SingleFloatMatcher(absl::string_view parameter_name, bool required,
                     bool inclusive, float_t minimum, float_t maximum,
                     absl::optional<float_t> default_value);
  const absl::optional<float_t>& Get() const;

 protected:
  void Match(ParameterData& data) final;

 private:
  bool ValidateFloat(float_t value) const;

  float_t m_minimum;
  float_t m_maximum;
  absl::optional<float_t> m_value;
  bool m_inclusive;
};

}  // namespace iris

#endif  // _SRC_PARAM_MATCHER_FLOAT_SINGLE_