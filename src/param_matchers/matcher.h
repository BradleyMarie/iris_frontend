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

template <typename ParameterForwardIterator,
          typename SupportedParameterForwardIterator>
void MatchParameters(
    const char* base_type_name, const char* type_name,
    ParameterForwardIterator& parameters_current,
    const ParameterForwardIterator& parameters_end,
    SupportedParameterForwardIterator supported_parameters_begin,
    const SupportedParameterForwardIterator& supported_parameters_end) {
  for (; parameters_current != parameters_end; ++parameters_current) {
    bool found = false;
    for (auto current = supported_parameters_begin;
         current != supported_parameters_end; ++current) {
      if ((*current)->Match(*parameters_current)) {
        found = true;
        break;
      }
    }

    if (!found) {
      std::cerr << "ERROR: Unrecognized or misconfigured parameter to "
                << type_name << " " << base_type_name << ": "
                << parameters_current->first << std::endl;
      exit(EXIT_FAILURE);
    }
  }
}

template <size_t NumParams>
void MatchParameters(
    const char* base_type_name, const char* type_name, Tokenizer& tokenizer,
    std::array<ParamMatcher*, NumParams> supported_parameters) {
  class Iterator {
   public:
    Iterator(Tokenizer& tokenizer, bool initialize_value)
        : m_tokenizer(tokenizer) {
      if (initialize_value) {
        m_value = ParseNextParam(m_tokenizer);
      }
    }

    bool operator!=(const Iterator& other) const {
      return m_value.has_value() && !other.m_value.has_value();
    }

    Iterator& operator++() {
      m_value = ParseNextParam(m_tokenizer);
      return *this;
    }

    Parameter& operator*() { return *m_value; }
    Parameter* operator->() { return &(*m_value); }

   private:
    absl::optional<Parameter> m_value;
    std::reference_wrapper<Tokenizer> m_tokenizer;
  };

  Iterator params_start(tokenizer, true);
  Iterator params_end(tokenizer, false);
  auto supported_start = supported_parameters.begin();
  auto supported_end = supported_parameters.end();

  MatchParameters<Iterator,
                  typename std::array<ParamMatcher*, NumParams>::iterator>(
      base_type_name, type_name, params_start, params_end, supported_start,
      supported_end);
}

template <size_t NumParams>
void MatchParameters(
    const char* base_type_name, const char* type_name,
    std::vector<Parameter>& parameters,
    std::array<ParamMatcher*, NumParams> supported_parameters) {
  auto params_start = parameters.begin();
  auto params_end = parameters.end();
  auto supported_start = supported_parameters.begin();
  auto supported_end = supported_parameters.end();

  MatchParameters<typename std::vector<Parameter>::iterator,
                  typename std::array<ParamMatcher*, NumParams>::iterator>(
      base_type_name, type_name, params_start, params_end, supported_start,
      supported_end);
}

}  // namespace iris

#endif  // _SRC_PARAM_MATCHER_MATCHER_