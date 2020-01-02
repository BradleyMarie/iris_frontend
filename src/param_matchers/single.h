#ifndef _SRC_PARAM_MATCHER_SINGLE_
#define _SRC_PARAM_MATCHER_SINGLE_

#include "src/param_matchers/matcher.h"

namespace iris {

template <typename VariantType, typename ValueType>
class SingleValueMatcher : public ParamMatcher {
 public:
  SingleValueMatcher(const char* base_type_name, const char* type_name,
                     const char* parameter_name, bool required,
                     ValueType default_value)
      : ParamMatcher(base_type_name, type_name, parameter_name, required,
                     &m_variant_type, 1),
        m_value(std::move(default_value)) {}
  const ValueType& Get() { return m_value; }

 protected:
  void Match(ParameterData& data) final {
    if (absl::get<VariantType>(data).data.size() != 1) {
      NumberOfElementsError();
    }
    m_value = std::move(absl::get<VariantType>(data).data[0]);
  }

 private:
  static const size_t m_variant_type;
  ValueType m_value;
};

template <typename VariantType, typename ValueType>
const size_t SingleValueMatcher<VariantType, ValueType>::m_variant_type =
    GetIndex<VariantType, ParameterData>();

typedef SingleValueMatcher<BoolParameter, bool> SingleBoolMatcher;
typedef SingleValueMatcher<StringParameter, std::string> SingleStringMatcher;

}  // namespace iris

#endif  // _SRC_PARAM_MATCHER_SINGLE_