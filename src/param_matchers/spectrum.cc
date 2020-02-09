#include "src/param_matchers/spectrum.h"

#include "absl/strings/str_join.h"
#include "src/param_matchers/spd_file.h"

namespace iris {

const size_t SpectrumMatcher::m_variant_indices[3] = {
    GetIndex<RgbParameter, ParameterData>(),
    GetIndex<SpectrumParameter, ParameterData>(),
    GetIndex<XyzParameter, ParameterData>()};

SpectrumMatcher SpectrumMatcher::FromRgb(
    const char* base_type_name, const char* type_name,
    const char* parameter_name, bool required,
    SpectrumManager& spectrum_manager,
    const std::array<float_t, 3>& default_rgb) {
  COLOR3 color = ColorCreate(COLOR_SPACE_LINEAR_SRGB, default_rgb.data());
  return SpectrumMatcher(base_type_name, type_name, parameter_name, required,
                         spectrum_manager,
                         spectrum_manager.AllocateColorSpectrum(color).value());
}

Spectrum SpectrumMatcher::Match(const RgbParameter& parameter) {
  if (parameter.data.size() != 1) {
    NumberOfElementsError();
  }
  COLOR3 color = ColorCreate(COLOR_SPACE_LINEAR_SRGB, parameter.data[0].data());
  return m_spectrum_manager.AllocateColorSpectrum(color).value();
}

Spectrum SpectrumMatcher::Match(const std::vector<std::string>& files) {
  if (files.size() != 1) {
    NumberOfElementsError();
  }
  auto maybe_samples = ReadSpdFile(/*search_dir=*/"", files[0]);
  if (!maybe_samples) {
    std::cerr << "ERROR: Malformed SPD file: " << files[0] << std::endl;
    exit(EXIT_FAILURE);
  }
  auto maybe_spectrum =
      m_spectrum_manager.AllocateInterpolatedSpectrum(*maybe_samples);
  if (!maybe_samples) {
    std::cerr << "ERROR: Malformed SPD file: " << files[0] << std::endl;
    exit(EXIT_FAILURE);
  }
  return *maybe_spectrum;
}

Spectrum SpectrumMatcher::Match(
    const std::pair<std::vector<std::string>, std::vector<float_t>>& samples) {
  if (samples.second.size() % 2 != 0) {
    NumberOfElementsError();
  }
  auto result = m_spectrum_manager.AllocateInterpolatedSpectrum(samples.second);
  if (!result) {
    std::cerr << "ERROR: Could not construct a spectrum from values ("
              << absl::StrJoin(samples.first, ", ") << ")" << std::endl;
    exit(EXIT_FAILURE);
  }
  return *result;
}

Spectrum SpectrumMatcher::Match(const SpectrumParameter& parameter) {
  if (absl::holds_alternative<std::vector<std::string>>(parameter.data)) {
    return Match(absl::get<std::vector<std::string>>(parameter.data));
  } else {
    return Match(
        absl::get<std::pair<std::vector<std::string>, std::vector<float_t>>>(
            parameter.data));
  }
}

Spectrum SpectrumMatcher::Match(const XyzParameter& parameter) {
  if (parameter.data.size() != 1) {
    NumberOfElementsError();
  }
  return m_spectrum_manager.AllocateColorSpectrum(parameter.data[0]).value();
}

void SpectrumMatcher::Match(ParameterData& data) {
  if (absl::holds_alternative<RgbParameter>(data)) {
    m_value = Match(absl::get<RgbParameter>(data));
  } else if (absl::holds_alternative<SpectrumParameter>(data)) {
    m_value = Match(absl::get<SpectrumParameter>(data));
  } else {
    m_value = Match(absl::get<XyzParameter>(data));
  }
}

}  // namespace iris