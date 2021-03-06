#ifndef _SRC_PARAM_MATCHER_SINGLE_
#define _SRC_PARAM_MATCHER_SINGLE_

#include "src/common/error.h"
#include "src/common/parameter_matcher.h"

namespace iris {

template <typename VariantType, typename ValueType>
class SingleValueMatcher : public ParameterMatcher {
 public:
  SingleValueMatcher(absl::string_view parameter_name, bool required,
                     ValueType default_value)
      : ParameterMatcher(parameter_name, required, m_variant_type),
        m_value(std::move(default_value)) {}
  const ValueType& Get() const { return m_value; }

 protected:
  void Match(ParameterData& data) final {
    if (absl::get<VariantType>(data).data.size() != 1) {
      NumberOfElementsError();
    }
    m_value = std::move(absl::get<VariantType>(data).data[0]);
  }

 private:
  static const size_t m_variant_type[1];
  ValueType m_value;
};

template <typename VariantType, typename ValueType>
const size_t SingleValueMatcher<VariantType, ValueType>::m_variant_type[1] = {
    GetIndex<VariantType, ParameterData>()};

typedef SingleValueMatcher<BoolParameter, bool> SingleBoolMatcher;
typedef SingleValueMatcher<StringParameter, std::string> SingleStringMatcher;
typedef SingleValueMatcher<Point3Parameter, POINT3> SinglePoint3Matcher;

}  // namespace iris

#endif  // _SRC_PARAM_MATCHER_SINGLE_