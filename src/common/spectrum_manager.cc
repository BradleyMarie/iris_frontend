#include "src/common/spectrum_manager.h"

#include "iris_physx_toolkit/color_spectra.h"
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

}  // namespace

SpectrumManager::SpectrumManager(ColorExtrapolator color_extrapolator,
                                 ColorIntegrator color_integrator,
                                 bool spectral)
    : m_color_extrapolator(std::move(color_extrapolator)),
      m_color_integrator(std::move(color_integrator)),
      m_spectral(spectral) {
  if (!spectral) {
    ISTATUS status = ColorColorExtrapolatorAllocate(
        COLOR_SPACE_LINEAR_SRGB,
        m_color_extrapolator.release_and_get_address());
    SuccessOrOOM(status);
  }
}

ColorExtrapolator& SpectrumManager::GetColorExtrapolator() {
  return m_color_extrapolator;
}

ColorIntegrator SpectrumManager::GetColorIntegrator() const {
  if (m_spectral) {
    return m_color_integrator;
  }

  ColorIntegrator result;
  ISTATUS status = ColorColorIntegratorAllocate(
      COLOR_SPACE_LINEAR_SRGB, result.release_and_get_address());
  SuccessOrOOM(status);

  return result;
}

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

  if (!m_spectral) {
    COLOR3 color;
    status = ColorIntegratorComputeSpectrumColor(m_color_integrator.get(),
                                                 result.get(), &color);
    if (status != ISTATUS_SUCCESS) {
      if (status == ISTATUS_ALLOCATION_FAILED) {
        ReportOOM();
      }
      return absl::nullopt;
    }

    result = AllocateColorSpectrum(color).value();
  }

  m_interpolated_spectra[wavelengths_and_intensities] = result;
  return result;
}

absl::optional<Spectrum> SpectrumManager::AllocateColorSpectrum(
    const COLOR3& color) {
  Spectrum result;
  ISTATUS status = ColorExtrapolatorComputeSpectrum(
      m_color_extrapolator.get(), color, result.release_and_get_address());
  if (status != ISTATUS_SUCCESS) {
    if (status == ISTATUS_ALLOCATION_FAILED) {
      ReportOOM();
    }
    return absl::nullopt;
  }

  return result;
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

  if (!m_spectral) {
    COLOR3 color;
    status = ColorIntegratorComputeReflectorColor(m_color_integrator.get(),
                                                  result.get(), &color);
    if (status != ISTATUS_SUCCESS) {
      if (status == ISTATUS_ALLOCATION_FAILED) {
        ReportOOM();
      }
      return absl::nullopt;
    }

    result = AllocateColorReflector(color).value();
  }

  m_interpolated_reflectors[wavelengths_and_reflectances] = result;
  return result;
}

absl::optional<Reflector> SpectrumManager::AllocateColorReflector(
    const COLOR3& color) {
  Reflector result;
  ISTATUS status = ColorExtrapolatorComputeReflector(
      m_color_extrapolator.get(), color, result.release_and_get_address());
  if (status != ISTATUS_SUCCESS) {
    if (status == ISTATUS_ALLOCATION_FAILED) {
      ReportOOM();
    }
    return absl::nullopt;
  }

  return result;
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

  if (!m_spectral) {
    COLOR3 color;
    status = ColorIntegratorComputeReflectorColor(m_color_integrator.get(),
                                                  result.get(), &color);
    if (status != ISTATUS_SUCCESS) {
      if (status == ISTATUS_ALLOCATION_FAILED) {
        ReportOOM();
      }
      return absl::nullopt;
    }

    result = AllocateColorReflector(color).value();
  }

  m_uniform_reflector[reflectance] = result;
  return result;
}

}  // namespace iris