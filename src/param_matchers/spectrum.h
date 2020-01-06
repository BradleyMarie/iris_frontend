#ifndef _SRC_PARAM_MATCHER_SPECTRUM_
#define _SRC_PARAM_MATCHER_SPECTRUM_

#include "src/common/pointer_types.h"
#include "src/common/spectrum_manager.h"
#include "src/param_matchers/matcher.h"

namespace iris {

class SpectrumMatcher : public ParamMatcher {
 public:
  SpectrumMatcher(const char* base_type_name, const char* type_name,
                  const char* parameter_name, bool required,
                  SpectrumManager& spectrum_manager, Spectrum default_value)
      : ParamMatcher(base_type_name, type_name, parameter_name, required,
                     m_variant_indices, 3),
        m_spectrum_manager(spectrum_manager),
        m_value(std::move(default_value)) {}
  const Spectrum& Get() { return m_value; }

  static SpectrumMatcher FromRgb(const char* base_type_name,
                                 const char* type_name,
                                 const char* parameter_name, bool required,
                                 SpectrumManager& spectrum_manager,
                                 const std::array<float_t, 3>& default_rgb);

 protected:
  void Match(ParameterData& data) final;

 private:
  Spectrum Match(const RgbParameter& parameter);
  Spectrum Match(const SpectrumParameter& parameter);
  Spectrum Match(const XyzParameter& parameter);

  Spectrum Match(const std::vector<std::string>& files);
  Spectrum Match(
      const std::pair<std::vector<std::string>, std::vector<float_t>>& samples);

 private:
  static const size_t m_variant_indices[3];
  SpectrumManager& m_spectrum_manager;
  Spectrum m_value;
};

}  // namespace iris

#endif  // _SRC_PARAM_MATCHER_SPECTRUM_