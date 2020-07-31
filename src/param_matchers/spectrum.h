#ifndef _SRC_PARAM_MATCHER_SPECTRUM_
#define _SRC_PARAM_MATCHER_SPECTRUM_

#include "src/common/parameter_matcher.h"
#include "src/common/pointer_types.h"
#include "src/common/spectrum_manager.h"

namespace iris {

class SpectrumMatcher : public ParameterMatcher {
 public:
  SpectrumMatcher(absl::string_view parameter_name, bool required,
                  SpectrumManager& spectrum_manager, Spectrum default_value);

  const Spectrum& Get() const;

  static SpectrumMatcher FromRgb(absl::string_view parameter_name,
                                 bool required,
                                 SpectrumManager& spectrum_manager,
                                 const std::array<float_t, 3>& default_rgb);

 protected:
  void Match(ParameterData& data) final;

 private:
  Spectrum Match(const ColorParameter& parameter);
  Spectrum Match(const SpectrumParameter& parameter);

  Spectrum Match(const std::vector<std::pair<std::string, std::string>>& files);
  Spectrum Match(
      const std::pair<std::vector<std::string>, std::vector<float_t>>& samples);

 private:
  static const size_t m_variant_indices[2];
  SpectrumManager& m_spectrum_manager;
  Spectrum m_value;
};

}  // namespace iris

#endif  // _SRC_PARAM_MATCHER_SPECTRUM_