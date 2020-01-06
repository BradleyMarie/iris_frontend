#include "src/common/spectrum_manager.h"

#include "iris_physx_toolkit/interpolated_spectrum.h"
#include "iris_physx_toolkit/uniform_reflector.h"
#include "src/common/error.h"

namespace iris {
namespace {

std::pair<std::vector<float_t>, std::vector<float_t>> UnswizzleFloats(
    const std::vector<float_t>& swizzled) {
  assert(swizzled.size() % 2 == 0);

  std::vector<float_t> first;
  std::vector<float_t> second;
  for (size_t i = 0; i < swizzled.size(); i += 2) {
    first.push_back(swizzled[i]);
    second.push_back(swizzled[i + 1]);
  }

  return std::make_pair(std::move(first), std::move(second));
}

std::array<float_t, 3> XyzToRgb(const COLOR3& color) {
  float_t r = (float_t)3.2404542 * color.x - (float_t)1.5371385 * color.y -
              (float_t)0.4985314 * color.z;

  float_t g = (float_t)-0.969266 * color.x + (float_t)1.8760108 * color.y +
              (float_t)0.0415560 * color.z;

  float_t b = (float_t)0.0556434 * color.x - (float_t)0.2040259 * color.y +
              (float_t)1.0572252 * color.z;

  r = std::max((float_t)0.0, r);
  g = std::max((float_t)0.0, g);
  b = std::max((float_t)0.0, b);

  return {r, g, b};
}

}  // namespace

absl::optional<Spectrum> SpectrumManager::AllocateInterpolatedSpectrum(
    const std::vector<float_t>& wavelengths_and_intensities) {
  if (wavelengths_and_intensities.size() % 2 != 0) {
    return absl::nullopt;
  }

  auto iter = m_interpolated_spectra.find(wavelengths_and_intensities);
  if (iter != m_interpolated_spectra.end()) {
    return iter->second;
  }

  auto unswizzled = UnswizzleFloats(wavelengths_and_intensities);

  Spectrum result;
  ISTATUS status = InterpolatedSpectrumAllocate(
      unswizzled.first.data(), unswizzled.second.data(),
      unswizzled.first.size(), result.release_and_get_address());
  if (status != ISTATUS_SUCCESS) {
    if (status == ISTATUS_ALLOCATION_FAILED) {
      ReportOOM();
    }
    return absl::nullopt;
  }

  m_interpolated_spectra[wavelengths_and_intensities] = result;
  return result;
}

absl::optional<Spectrum> SpectrumManager::AllocateRgbSpectrum(
    const std::array<float_t, 3>& rgb) {
  auto iter = m_rgb_spectra.find(rgb);
  if (iter != m_rgb_spectra.end()) {
    return iter->second;
  }

  Spectrum result;
  ISTATUS status = RgbInterpolatorAllocateSpectrum(
      m_color_extrapolator.get(), rgb[0], rgb[1], rgb[2],
      result.release_and_get_address());
  if (status != ISTATUS_SUCCESS) {
    if (status == ISTATUS_ALLOCATION_FAILED) {
      ReportOOM();
    }
    return absl::nullopt;
  }

  m_rgb_spectra[rgb] = result;
  return result;
}

absl::optional<Spectrum> SpectrumManager::AllocateXyzSpectrum(
    const COLOR3& color) {
  auto rgb = XyzToRgb(color);
  return AllocateRgbSpectrum(rgb);
}

absl::optional<Reflector> SpectrumManager::AllocateInterpolatedReflector(
    const std::vector<float_t>& wavelengths_and_reflectances) {
  if (wavelengths_and_reflectances.size() % 2 != 0) {
    return absl::nullopt;
  }

  auto iter = m_interpolated_reflectors.find(wavelengths_and_reflectances);
  if (iter != m_interpolated_reflectors.end()) {
    return iter->second;
  }

  auto unswizzled = UnswizzleFloats(wavelengths_and_reflectances);

  Reflector result;
  ISTATUS status = InterpolatedReflectorAllocate(
      unswizzled.first.data(), unswizzled.second.data(),
      unswizzled.first.size(), result.release_and_get_address());
  if (status != ISTATUS_SUCCESS) {
    if (status == ISTATUS_ALLOCATION_FAILED) {
      ReportOOM();
    }
    return absl::nullopt;
  }

  m_interpolated_reflectors[wavelengths_and_reflectances] = result;
  return result;
}

absl::optional<Reflector> SpectrumManager::AllocateRgbReflector(
    const std::array<float_t, 3>& rgb) {
  auto iter = m_rgb_reflectors.find(rgb);
  if (iter != m_rgb_reflectors.end()) {
    return iter->second;
  }

  Reflector result;
  ISTATUS status = RgbInterpolatorAllocateReflector(
      m_color_extrapolator.get(), rgb[0], rgb[1], rgb[2],
      result.release_and_get_address());
  if (status != ISTATUS_SUCCESS) {
    if (status == ISTATUS_ALLOCATION_FAILED) {
      ReportOOM();
    }
    return absl::nullopt;
  }

  m_rgb_reflectors[rgb] = result;
  return result;
}

absl::optional<Reflector> SpectrumManager::AllocateXyzReflector(
    const COLOR3& color) {
  auto rgb = XyzToRgb(color);
  return AllocateRgbReflector(rgb);
}

absl::optional<Reflector> SpectrumManager::AllocateUniformReflector(
    float_t reflectance) {
  auto iter = m_uniform_reflector.find(reflectance);
  if (iter != m_uniform_reflector.end()) {
    return iter->second;
  }

  Reflector result;
  ISTATUS status =
      UniformReflectorAllocate(reflectance, result.release_and_get_address());
  if (status != ISTATUS_SUCCESS) {
    if (status == ISTATUS_ALLOCATION_FAILED) {
      ReportOOM();
    }
    return absl::nullopt;
  }

  m_uniform_reflector[reflectance] = result;
  return result;
}

}  // namespace iris