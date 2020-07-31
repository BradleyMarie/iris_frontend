#ifndef _SRC_PARAM_MATCHER_LIST_
#define _SRC_PARAM_MATCHER_LIST_

#include "src/common/parameter_matcher.h"

namespace iris {

template <typename VariantType, typename ValueType, size_t Min, size_t Mod>
class ListValueMatcher : public ParamMatcher {
 public:
  ListValueMatcher(absl::string_view parameter_name, bool required,
                   std::vector<ValueType> default_value)
      : ParamMatcher(parameter_name, required, &m_variant_type, 1),
        m_value(std::move(default_value)) {}
  const std::vector<ValueType>& Get() { return m_value; }
  std::vector<ValueType>& GetMutable() { return m_value; }

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