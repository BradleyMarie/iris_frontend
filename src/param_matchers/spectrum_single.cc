#include "src/param_matchers/spectrum_single.h"

namespace iris {
namespace {

Spectrum SpectrumFromRgb(const ColorExtrapolator& color_extrapolator,
                         const std::array<float_t, 3>& rgb) {
  Spectrum result;
  ISTATUS status =
      RgbInterpolatorAllocateSpectrum(color_extrapolator.get(), rgb[0], rgb[1],
                                      rgb[2], result.release_and_get_address());
  SuccessOrOOM(status);
  return result;
}

}  // namespace

const size_t SingleSpectrumMatcher::m_variant_indices[3] = {
    GetIndex<RgbParameter, ParameterData>(),
    GetIndex<SpectrumParameter, ParameterData>(),
    GetIndex<XyzParameter, ParameterData>()};

SingleSpectrumMatcher SingleSpectrumMatcher::FromRgb(
    const char* base_type_name, const char* type_name,
    const char* parameter_name, bool required,
    const ColorExtrapolator& color_extrapolator,
    const std::array<float_t, 3>& default_rgb) {
  return SingleSpectrumMatcher(
      base_type_name, type_name, parameter_name, required, color_extrapolator,
      SpectrumFromRgb(color_extrapolator, default_rgb));
}

Spectrum SingleSpectrumMatcher::Match(const RgbParameter& parameter) const {
  if (parameter.data.size() != 1) {
    NumberOfElementsError();
  }
  return SpectrumFromRgb(m_color_extrapolator, parameter.data[0]);
}

Spectrum SingleSpectrumMatcher::Match(
    const SpectrumParameter& parameter) const {
  if (parameter.data.size() != 1) {
    NumberOfElementsError();
  }
  return parameter.data[0].first;
}

Spectrum SingleSpectrumMatcher::Match(const XyzParameter& parameter) const {
  if (parameter.data.size() != 1) {
    NumberOfElementsError();
  }

  float_t r = (float_t)3.2404542 * parameter.data[0].x -
              (float_t)1.5371385 * parameter.data[0].y -
              (float_t)0.4985314 * parameter.data[0].z;

  float_t g = (float_t)-0.969266 * parameter.data[0].x +
              (float_t)1.8760108 * parameter.data[0].y +
              (float_t)0.0415560 * parameter.data[0].z;

  float_t b = (float_t)0.0556434 * parameter.data[0].x -
              (float_t)0.2040259 * parameter.data[0].y +
              (float_t)1.0572252 * parameter.data[0].z;

  r = std::max((float_t)0.0, r);
  g = std::max((float_t)0.0, g);
  b = std::max((float_t)0.0, b);

  return SpectrumFromRgb(m_color_extrapolator, {r, g, b});
}

void SingleSpectrumMatcher::Match(ParameterData& data) {
  if (absl::holds_alternative<RgbParameter>(data)) {
    m_value = Match(absl::get<RgbParameter>(data));
  } else if (absl::holds_alternative<SpectrumParameter>(data)) {
    m_value = Match(absl::get<SpectrumParameter>(data));
  } else {
    m_value = Match(absl::get<XyzParameter>(data));
  }
}

}  // namespace iris