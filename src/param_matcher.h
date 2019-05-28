#ifndef _SRC_PARAM_MATCHER_
#define _SRC_PARAM_MATCHER_

#include <iostream>

#include "src/param_parser.h"

namespace iris {

class ParamMatcher {
 public:
  bool Match(const Parameter& parameter);

 protected:
  ParamMatcher(const char* base_type_name, const char* type_name,
               const char* parameter_name, size_t variant_index)
      : m_parameter_name(parameter_name),
        m_base_type_name(base_type_name),
        m_type_name(type_name),
        m_index(variant_index),
        m_found(false) {}

  virtual void Match(const ParameterData& data) = 0;
  void NumberOfElementsError[[noreturn]]();
  void ElementRangeError[[noreturn]]();

  template <typename T>
  size_t GetIndex() {
    ParameterData data = T();
    return data.index();
  }

 private:
  const char* m_parameter_name;
  const char* m_base_type_name;
  const char* m_type_name;
  size_t m_index;
  bool m_found;
};

template <typename VariantType, typename ValueType>
class SingleValueMatcher : public ParamMatcher {
 public:
  SingleValueMatcher(const char* base_type_name, const char* type_name,
                     const char* parameter_name, ValueType default_value)
      : ParamMatcher(base_type_name, type_name, parameter_name,
                     GetIndex<VariantType>()),
        m_value(default_value) {}
  const ValueType& Get() { return m_value; }

 private:
  void Match(const ParameterData& data) final {
    if (absl::get<VariantType>(data).data.size() != 1) {
      NumberOfElementsError();
    }
    m_value = static_cast<ValueType>(absl::get<VariantType>(data).data[0]);
  }

 private:
  ValueType m_value;
};

template <typename VariantType, typename ValueType, ValueType Minimum,
          ValueType Maximum>
class PreBoundedSingleValueMatcher : public ParamMatcher {
 public:
  PreBoundedSingleValueMatcher(const char* base_type_name,
                               const char* type_name,
                               const char* parameter_name,
                               ValueType default_value)
      : ParamMatcher(base_type_name, type_name, parameter_name,
                     GetIndex<VariantType>()),
        m_value(default_value) {}
  const ValueType& Get() { return m_value; }

 private:
  void Match(const ParameterData& data) final {
    if (absl::get<VariantType>(data).data.size() != 1) {
      NumberOfElementsError();
    }
    auto value = absl::get<VariantType>(data).data[0];
    if (value < Minimum || Maximum < value) {
      ElementRangeError();
    }
    m_value = static_cast<ValueType>(absl::get<VariantType>(data).data[0]);
  }

 private:
  ValueType m_value;
};

typedef SingleValueMatcher<IntParameter, bool> SingleBoolMatcher;

typedef PreBoundedSingleValueMatcher<IntParameter, uint16_t, 0, UINT16_MAX>
    SingleUInt16Matcher;

template <size_t NumParams>
void MatchParameter(
    const char* base_type_name, const char* type_name,
    const Parameter& parameter,
    const std::array<ParamMatcher*, NumParams>& supported_parameters) {
  for (auto& matcher : supported_parameters) {
    if (matcher->Match(parameter)) {
      break;
    }
  }

  std::cerr << "ERROR: Unrecognized or misconfigured parameter to " << type_name
            << " " << base_type_name << ": " << parameter.first << std::endl;
  exit(EXIT_FAILURE);
}

template <size_t NumParams>
void ParseAllParameter(
    const char* base_type_name, const char* type_name, Tokenizer& tokenizer,
    const std::array<ParamMatcher*, NumParams>& supported_parameters) {
  for (auto param = ParseNextParam(tokenizer); param;
       param = ParseNextParam(tokenizer)) {
    MatchParameter<NumParams>(base_type_name, type_name, *param,
                              supported_parameters);
  }
}

}  // namespace iris

#endif  // _SRC_PARAM_MATCHER_