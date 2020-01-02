#ifndef _SRC_PARAM_MATCHER_SPECTRUM_SINGLE_
#define _SRC_PARAM_MATCHER_SPECTRUM_SINGLE_

#include "src/common/error.h"
#include "src/common/pointer_types.h"
#include "src/param_matchers/matcher.h"

namespace iris {

class SingleSpectrumMatcher : public ParamMatcher {
 public:
  SingleSpectrumMatcher(const char* base_type_name, const char* type_name,
                        const char* parameter_name, bool required,
                        const ColorExtrapolator& color_extrapolator,
                        Spectrum default_value)
      : ParamMatcher(base_type_name, type_name, parameter_name, required,
                     m_variant_indices, 3),
        m_color_extrapolator(color_extrapolator),
        m_value(default_value) {}
  const Spectrum& Get() { return m_value; }

  static SingleSpectrumMatcher FromRgb(
      const char* base_type_name, const char* type_name,
      const char* parameter_name, bool required,
      const ColorExtrapolator& color_extrapolator,
      const std::array<float_t, 3>& default_rgb);

 private:
  void Match(ParameterData& data) final;

  Spectrum Match(const RgbParameter& parameter) const;
  Spectrum Match(const SpectrumParameter& parameter) const;
  Spectrum Match(const XyzParameter& parameter) const;

 private:
  static const size_t m_variant_indices[3];
  const ColorExtrapolator& m_color_extrapolator;
  Spectrum m_value;
};

}  // namespace iris

#endif  // _SRC_PARAM_MATCHER_SPECTRUM_SINGLE_