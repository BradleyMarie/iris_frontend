#ifndef _SRC_PARAM_MATCHER_LIST_
#define _SRC_PARAM_MATCHER_LIST_

#include "src/param_matchers/matcher.h"

namespace iris {

template <typename VariantType, typename ValueType, size_t Min, size_t Mod>
class ListValueMatcher : public ParamMatcher {
 public:
  ListValueMatcher(const char* base_type_name, const char* type_name,
                   const char* parameter_name, bool required,
                   std::vector<ValueType> default_value)
      : ParamMatcher(base_type_name, type_name, parameter_name, required,
                     &m_variant_type, 1),
        m_value(default_value) {}
  const std::vector<ValueType>& Get() { return m_value; }

 protected:
  void Match(ParameterData& data) final {
    if (absl::get<VariantType>(data).data.size() < Min ||
        absl::get<VariantType>(data).data.size() % Mod != 0) {
      NumberOfElementsError();
    }
    m_value = std::move(absl::get<VariantType>(data).data);
  }

 private:
  static const size_t m_variant_type;
  std::vector<ValueType> m_value;
};

template <typename VariantType, typename ValueType, size_t Min, size_t Mod>
const size_t
    ListValueMatcher<VariantType, ValueType, Min, Mod>::m_variant_type =
        GetIndex<VariantType, ParameterData>();

}  // namespace iris

#endif  // _SRC_PARAM_MATCHER_LIST_