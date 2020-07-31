#ifndef _SRC_COMMON_PARAMETER_MATCHER_
#define _SRC_COMMON_PARAMETER_MATCHER_

#include <type_traits>

#include "absl/types/span.h"
#include "absl/strings/string_view.h"
#include "src/common/parameter.h"

namespace iris {

class ParameterMatcher {
 public:
  bool Match(absl::string_view base_type_name,
             absl::optional<absl::string_view> type_name,
             Parameter& parameter);
  void Validate(absl::string_view base_type_name,
                absl::optional<absl::string_view> type_name) const;

 protected:
  ParameterMatcher(absl::string_view parameter_name, bool required,
                   absl::Span<const size_t> variant_indices);

  virtual void Match(ParameterData& data) = 0;
  void NumberOfElementsError [[noreturn]] () const;
  void ElementRangeError [[noreturn]] () const;

 private:
  absl::string_view m_parameter_name;
  absl::string_view m_base_type_name;
  absl::optional<absl::string_view> m_type_name;
  absl::Span<const size_t> m_variant_indices;
  bool m_required;
  bool m_found;
};

template <typename>
struct tag {};

template <typename T, typename V>
struct GetIndex;

template <typename T, typename... Ts>
struct GetIndex<T, absl::variant<Ts...>>
    : std::integral_constant<size_t,
                             absl::variant<tag<Ts>...>(tag<T>()).index()> {};

}  // namespace iris

#endif  // _SRC_COMMON_PARAMETER_MATCHER_