#ifndef _SRC_PARAM_MATCHER_MATCHER_
#define _SRC_PARAM_MATCHER_MATCHER_

#include <type_traits>
#include <vector>

#include "absl/types/span.h"
#include "src/param_matchers/parser.h"

namespace iris {

class ParamMatcher {
 public:
  bool Match(Parameter& parameter);
  void Validate() const;

 protected:
  ParamMatcher(absl::string_view base_type_name, absl::string_view type_name,
               absl::string_view parameter_name, bool required,
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
  absl::string_view m_parameter_name;
  absl::string_view m_base_type_name;
  absl::string_view m_type_name;
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
    absl::string_view base_type_name, absl::string_view type_name,
    ParameterForwardIterator parameters_current,
    ParameterForwardIterator parameters_end,
    SupportedParameterForwardIterator supported_parameters_begin,
    SupportedParameterForwardIterator supported_parameters_end,
    std::vector<Parameter>* unhandled_parameters) {
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
      if (!unhandled_parameters) {
        std::cerr << "ERROR: Unrecognized or misconfigured parameter to "
                  << type_name << " " << base_type_name << ": "
                  << parameters_current->first << std::endl;
        exit(EXIT_FAILURE);
      }

      unhandled_parameters->push_back(*parameters_current);
    }
  }
}

template <typename SupportedParameterForwardIterator>
void MatchParameters(
    absl::string_view base_type_name, absl::string_view type_name,
    Tokenizer& tokenizer,
    SupportedParameterForwardIterator supported_parameters_begin,
    SupportedParameterForwardIterator supported_parameters_end,
    std::vector<Parameter>* unhandled_parameters) {
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

  MatchParameters<Iterator, SupportedParameterForwardIterator>(
      base_type_name, type_name, Iterator(tokenizer, true),
      Iterator(tokenizer, false), supported_parameters_begin,
      supported_parameters_end, unhandled_parameters);
}

static inline void MatchParameters(
    absl::string_view base_type_name, absl::string_view type_name,
    Tokenizer& tokenizer, absl::Span<ParamMatcher* const> supported_parameters,
    std::vector<Parameter>* unhandled_parameters = nullptr) {
  MatchParameters<typename absl::Span<ParamMatcher* const>::iterator>(
      base_type_name, type_name, tokenizer, supported_parameters.begin(),
      supported_parameters.end(), unhandled_parameters);
}

static inline void MatchParameters(
    absl::string_view base_type_name, absl::string_view type_name,
    absl::Span<Parameter> parameters,
    absl::Span<ParamMatcher* const> supported_parameters,
    std::vector<Parameter>* unhandled_parameters = nullptr) {
  MatchParameters<typename absl::Span<Parameter>::iterator,
                  typename absl::Span<ParamMatcher* const>::iterator>(
      base_type_name, type_name, parameters.begin(), parameters.end(),
      supported_parameters.begin(), supported_parameters.end(),
      unhandled_parameters);
}

}  // namespace iris

#endif  // _SRC_PARAM_MATCHER_MATCHER_