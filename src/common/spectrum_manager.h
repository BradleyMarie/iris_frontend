#ifndef _SRC_COMMON_SPECTRUM_MANAGER_
#define _SRC_COMMON_SPECTRUM_MANAGER_

#include <map>
#include <vector>

#include "absl/types/optional.h"
#include "src/common/pointer_types.h"

namespace iris {

class SpectrumManager {
 public:
  SpectrumManager(ColorExtrapolator color_extrapolator,
                  ColorIntegrator color_integrator, bool spectral);

  ColorIntegrator GetColorIntegrator() const;
  ColorExtrapolator& GetColorExtrapolator();

  absl::optional<Spectrum> AllocateInterpolatedSpectrum(
      const std::vector<float_t>& wavelengths_and_intensities);
  absl::optional<Spectrum> AllocateColorSpectrum(const COLOR3& color);

  absl::optional<Reflector> AllocateInterpolatedReflector(
      const std::vector<float_t>& wavelengths_and_reflectances);
  absl::optional<Reflector> AllocateColorReflector(const COLOR3& color);
  absl::optional<Reflector> AllocateUniformReflector(float_t reflectance);

 private:
  ColorExtrapolator m_color_extrapolator;
  ColorIntegrator m_color_integrator;
  bool m_spectral;

  std::map<std::vector<float_t>, Spectrum> m_interpolated_spectra;
  std::map<std::vector<float_t>, Reflector> m_interpolated_reflectors;
  std::map<float_t, Reflector> m_uniform_reflector;
};

}  // namespace iris

#endif  // _SRC_COMMON_SPECTRUM_MANAGER_