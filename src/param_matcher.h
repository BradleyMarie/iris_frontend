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

template <typename VariantType, typename ValueType, size_t Min, size_t Mod>
class ListValueMatcher : public ParamMatcher {
 public:
  ListValueMatcher(const char* base_type_name, const char* type_name,
                   const char* parameter_name,
                   std::vector<ValueType> default_value)
      : ParamMatcher(base_type_name, type_name, parameter_name,
                     GetIndex<VariantType>()),
        m_value(default_value) {}
  const std::vector<ValueType>& Get() { return m_value; }

 private:
  void Match(const ParameterData& data) final {
    if (absl::get<VariantType>(data).data.size() < Min ||
        absl::get<VariantType>(data).data.size() % Mod != 0) {
      NumberOfElementsError();
    }
    m_value = absl::get<VariantType>(data).data;
  }

 private:
  std::vector<ValueType> m_value;
};

template <typename VariantType, typename ValueType, typename ComparisonType,
          ComparisonType Minimum, ComparisonType Maximum, size_t Min,
          size_t Mod>
class PreBoundedListValueMatcher : public ParamMatcher {
 public:
  PreBoundedListValueMatcher(const char* base_type_name, const char* type_name,
                             const char* parameter_name,
                             std::vector<ValueType> default_value)
      : ParamMatcher(base_type_name, type_name, parameter_name,
                     GetIndex<VariantType>()),
        m_value(default_value) {}
  const std::vector<ValueType>& Get() { return m_value; }

 private:
  void Match(const ParameterData& data) final {
    if (absl::get<VariantType>(data).data.size() < Min ||
        absl::get<VariantType>(data).data.size() % Mod != 0) {
      NumberOfElementsError();
    }
    m_value.reserve(absl::get<VariantType>(data).data.size());
    for (const auto& value : absl::get<VariantType>(data).data) {
      if (value < Minimum || Maximum < value) {
        ElementRangeError();
      }
      m_value.push_back(static_cast<ValueType>(value));
    }
  }

 private:
  std::vector<ValueType> m_value;
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

typedef SingleValueMatcher<BoolParameter, bool> SingleBoolMatcher;

template <typename VariantType, typename ValueType>
class SingleValueMoveMatcher : public ParamMatcher {
 public:
  SingleValueMoveMatcher(const char* base_type_name, const char* type_name,
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
    m_value = std::move(absl::get<VariantType>(data).data[0]);
  }

 private:
  ValueType m_value;
};

typedef SingleValueMoveMatcher<StringParameter, std::string>
    SingleStringMatcher;

template <typename VariantType, typename ValueType, typename ComparisonType,
          ComparisonType Minimum, ComparisonType Maximum>
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

typedef PreBoundedSingleValueMatcher<IntParameter, uint8_t, int, 1, UINT8_MAX>
    NonZeroSingleUInt8Matcher;
typedef PreBoundedSingleValueMatcher<IntParameter, uint16_t, int, 1, UINT16_MAX>
    NonZeroSingleUInt16Matcher;
typedef PreBoundedSingleValueMatcher<IntParameter, size_t, int, 1, INT_MAX>
    NonZeroSingleSizeTMatcher;

class PositiveFiniteSingleFloatTMatcher : public ParamMatcher {
 public:
  PositiveFiniteSingleFloatTMatcher(const char* base_type_name,
                                    const char* type_name,
                                    const char* parameter_name,
                                    float_t default_value)
      : ParamMatcher(base_type_name, type_name, parameter_name,
                     GetIndex<FloatParameter>()),
        m_value(default_value) {}
  const float_t& Get() { return m_value; }

 private:
  void Match(const ParameterData& data) final {
    if (absl::get<FloatParameter>(data).data.size() != 1) {
      NumberOfElementsError();
    }
    auto value = absl::get<FloatParameter>(data).data[0];
    if (value < (float_t)0.0 || !isfinite(value)) {
      ElementRangeError();
    }
    m_value = static_cast<float_t>(absl::get<FloatParameter>(data).data[0]);
  }

 private:
  float_t m_value;
};

class PositiveBoundedSingleFloatTMatcher : public ParamMatcher {
 public:
  PositiveBoundedSingleFloatTMatcher(const char* base_type_name,
                                     const char* type_name,
                                     const char* parameter_name,
                                     float_t max_value, float_t default_value)
      : ParamMatcher(base_type_name, type_name, parameter_name,
                     GetIndex<FloatParameter>()),
        m_max_value(max_value),
        m_value(default_value) {
    assert((float_t)0.0 < max_value && std::isfinite(max_value));
  }
  const float_t& Get() { return m_value; }

 private:
  void Match(const ParameterData& data) final {
    if (absl::get<FloatParameter>(data).data.size() != 1) {
      NumberOfElementsError();
    }
    auto value = absl::get<FloatParameter>(data).data[0];
    if (value < (float_t)0.0 || m_max_value < value) {
      ElementRangeError();
    }
    m_value = static_cast<float_t>(absl::get<FloatParameter>(data).data[0]);
  }

 private:
  float_t m_max_value;
  float_t m_value;
};

class PositiveScalarSingleFloatTMatcher
    : public PositiveBoundedSingleFloatTMatcher {
 public:
  PositiveScalarSingleFloatTMatcher(const char* base_type_name,
                                    const char* type_name,
                                    const char* parameter_name,
                                    float_t default_value)
      : PositiveBoundedSingleFloatTMatcher(base_type_name, type_name,
                                           parameter_name, (float_t)1.0,
                                           default_value) {}
};

template <size_t NumParams>
void MatchParameter(
    const char* base_type_name, const char* type_name,
    const Parameter& parameter,
    const std::array<ParamMatcher*, NumParams>& supported_parameters) {
  for (auto& matcher : supported_parameters) {
    if (matcher->Match(parameter)) {
      return;
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