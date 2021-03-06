#ifndef _SRC_PARAM_MATCHER_INTEGRAL_SINGLE_
#define _SRC_PARAM_MATCHER_INTEGRAL_SINGLE_

#include "src/common/error.h"
#include "src/common/parameter_matcher.h"

namespace iris {

template <typename ValueType, int Minimum, ValueType Maximum>
class IntegralSingleValueMatcher : public ParameterMatcher {
 public:
  IntegralSingleValueMatcher(absl::string_view parameter_name, bool required,
                             ValueType default_value)
      : ParameterMatcher(parameter_name, required, m_variant_type),
        m_value(std::move(default_value)) {}
  const ValueType& Get() const { return m_value; }

 protected:
  static_assert(Minimum < Maximum);

  void Match(ParameterData& data) final {
    if (absl::get<IntParameter>(data).data.size() != 1) {
      NumberOfElementsError();
    }
    auto value = absl::get<IntParameter>(data).data[0];
    if (value < Minimum || m_maximum < value) {
      ElementRangeError();
    }
    m_value = static_cast<ValueType>(absl::get<IntParameter>(data).data[0]);
  }

  static const size_t m_variant_type[1];
  static const int m_maximum;
  ValueType m_value;
};

template <typename ValueType, int Minimum, ValueType Maximum>
const size_t IntegralSingleValueMatcher<ValueType, Minimum,
                                        Maximum>::m_variant_type[1] = {
    GetIndex<IntParameter, ParameterData>()};

template <typename ValueType, int Minimum, ValueType Maximum>
const int IntegralSingleValueMatcher<ValueType, Minimum, Maximum>::m_maximum =
    (INT32_MAX < Maximum) ? INT32_MAX : Maximum;

typedef IntegralSingleValueMatcher<uint8_t, 1, UINT8_MAX>
    NonZeroSingleUInt8Matcher;
typedef IntegralSingleValueMatcher<uint16_t, 1, UINT16_MAX>
    NonZeroSingleUInt16Matcher;
typedef IntegralSingleValueMatcher<size_t, 1, SIZE_MAX>
    NonZeroSingleSizeTMatcher;

}  // namespace iris

#endif  // _SRC_PARAM_MATCHER_INTEGRAL_SINGLE_