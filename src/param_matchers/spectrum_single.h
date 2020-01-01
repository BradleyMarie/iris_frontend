#ifndef _SRC_PARAM_MATCHER_SPECTRUM_SINGLE_
#define _SRC_PARAM_MATCHER_SPECTRUM_SINGLE_

#include "src/common/pointer_types.h"
#include "src/param_matchers/matcher.h"

namespace iris {

class SingleSpectrumMatcher : public ParamMatcher {
 public:
  SingleSpectrumMatcher(const char* base_type_name, const char* type_name,
                        const char* parameter_name, bool required,
                        Spectrum default_value)
      : ParamMatcher(base_type_name, type_name, parameter_name, required,
                     &m_variant_index, 1),
        m_value(default_value) {}
  const Spectrum& Get() { return m_value; }

 private:
  void Match(ParameterData& data) final {
    if (absl::get<SpectrumParameter>(data).data.size() != 1) {
      NumberOfElementsError();
    }
    m_value = absl::get<SpectrumParameter>(data).data[0].first;
  }

 private:
  static const size_t m_variant_index;
  Spectrum m_value;
};

}  // namespace iris

#endif  // _SRC_PARAM_MATCHER_SPECTRUM_SINGLE_