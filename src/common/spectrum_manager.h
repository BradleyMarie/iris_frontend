#ifndef _SRC_COMMON_SPECTRUM_MANAGER_
#define _SRC_COMMON_SPECTRUM_MANAGER_

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "absl/types/optional.h"
#include "src/common/pointer_types.h"

namespace iris {

class SpectrumManager {
 public:
  SpectrumManager(ColorExtrapolator&& color_extrapolator)
      : m_color_extrapolator(std::move(m_color_extrapolator)) {}

  absl::optional<Spectrum> AllocateInterpolatedSpectrum(
      const std::vector<float_t>& wavelengths_and_intensities);
  absl::optional<Spectrum> AllocateRgbSpectrum(
      const std::array<float_t, 3>& rgb);
  absl::optional<Spectrum> AllocateXyzSpectrum(const COLOR3& color);

  absl::optional<Reflector> AllocateInterpolatedReflector(
      const std::vector<float_t>& wavelengths_and_reflectances);
  absl::optional<Reflector> AllocateRgbReflector(
      const std::array<float_t, 3>& rgb);
  absl::optional<Reflector> AllocateXyzReflector(const COLOR3& color);

 private:
  ColorExtrapolator m_color_extrapolator;
  std::map<std::vector<float_t>, Spectrum> m_interpolated_spectra;
  std::map<std::array<float_t, 3>, Spectrum> m_rgb_spectra;
  std::map<std::vector<float_t>, Reflector> m_interpolated_reflectors;
  std::map<std::array<float_t, 3>, Reflector> m_rgb_reflectors;
};

}  // namespace iris

#endif  // _SRC_COMMON_SPECTRUM_MANAGER_