#ifndef _SRC_PARAM_MATCHER_MATCHER_
#define _SRC_PARAM_MATCHER_MATCHER_

#include <type_traits>

#include "src/param_matchers/parser.h"

namespace iris {

class ParamMatcher {
 public:
  bool Match(Parameter& parameter);
  void Validate() const;

 protected:
  ParamMatcher(const char* base_type_name, const char* type_name,
               const char* parameter_name, bool required,
               const size_t* variant_indicies, size_t num_indices)
      : m_parameter_name(parameter_name),
        m_base_type_name(base_type_name),
        m_type_name(type_name),
        m_indices(variant_indicies),
        m_num_indices(num_indices),
        m_required(required),
        m_found(false) {}

  virtual void Match(ParameterData& data) = 0;
  void NumberOfElementsError [[noreturn]] () const;
  void ElementRangeError [[noreturn]] () const;

 private:
  const char* m_parameter_name;
  const char* m_base_type_name;
  const char* m_type_name;
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

template <size_t NumParams>
void MatchParameters(
    const char* base_type_name, const char* type_name, Tokenizer& tokenizer,
    const std::array<ParamMatcher*, NumParams>& supported_parameters) {
  for (auto param = ParseNextParam(tokenizer); param;
       param = ParseNextParam(tokenizer)) {
    bool found = false;
    for (auto& matcher : supported_parameters) {
      if (matcher->Match(*param)) {
        found = true;
        break;
      }
    }

    if (!found) {
      std::cerr << "ERROR: Unrecognized or misconfigured parameter to "
                << type_name << " " << base_type_name << ": " << param->first
                << std::endl;
      exit(EXIT_FAILURE);
    }
  }
}

}  // namespace iris

#endif  // _SRC_PARAM_MATCHER_MATCHER_