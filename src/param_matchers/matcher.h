#ifndef _SRC_PARAM_MATCHER_MATCHER_
#define _SRC_PARAM_MATCHER_MATCHER_

#include <type_traits>

#include "src/param_matchers/parser.h"

namespace iris {

class ParamMatcher {
 public:
  bool Match(absl::string_view base_type_name,
             absl::optional<absl::string_view> type_name,
             Parameter& parameter);
  void Validate(absl::string_view base_type_name,
                absl::optional<absl::string_view> type_name) const;

 protected:
  ParamMatcher(absl::string_view parameter_name, bool required,
               const size_t* variant_indicies, size_t num_indices)
      : m_parameter_name(parameter_name),
        m_indices(variant_indicies),
        m_num_indices(num_indices),
        m_required(required),
        m_found(false) {}

  virtual void Match(ParameterData& data) = 0;
  void NumberOfElementsError [[noreturn]] () const;
  void ElementRangeError [[noreturn]] () const;

 private:
  absl::string_view m_parameter_name;
  absl::string_view m_base_type_name;
  absl::optional<absl::string_view> m_type_name;
  const size_t* m_indices;
  size_t m_num_indices;
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

#endif  // _SRC_PARAM_MATCHER_MATCHER_