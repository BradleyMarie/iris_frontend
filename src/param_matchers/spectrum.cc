#include "src/param_matchers/spectrum.h"

#include "absl/strings/str_join.h"
#include "src/common/flags.h"
#include "src/param_matchers/spd_file.h"

namespace iris {

const size_t SpectrumMatcher::m_variant_indices[2] = {
    GetIndex<ColorParameter, ParameterData>(),
    GetIndex<SpectrumParameter, ParameterData>()};

SpectrumMatcher SpectrumMatcher::FromRgb(
    absl::string_view parameter_name, bool required,
    SpectrumManager& spectrum_manager,
    const std::array<float_t, 3>& default_rgb) {
  COLOR3 color =
      ColorCreate(absl::GetFlag(FLAGS_rgb_color_space), default_rgb.data());
  return SpectrumMatcher(parameter_name, required, spectrum_manager,
                         spectrum_manager.AllocateColorSpectrum(color).value());
}

Spectrum SpectrumMatcher::Match(const ColorParameter& parameter) {
  if (parameter.data.size() != 1) {
    NumberOfElementsError();
  }
  return m_spectrum_manager.AllocateColorSpectrum(parameter.data[0]).value();
}

Spectrum SpectrumMatcher::Match(
    const std::vector<std::pair<std::string, std::string>>& files) {
  if (files.size() != 1) {
    NumberOfElementsError();
  }
  auto maybe_samples = ReadSpdFile(files[0].first, files[0].second);
  if (!maybe_samples) {
    std::cerr << "ERROR: Malformed SPD file: " << files[0].first << std::endl;
    exit(EXIT_FAILURE);
  }
  auto maybe_spectrum =
      m_spectrum_manager.AllocateInterpolatedSpectrum(*maybe_samples);
  if (!maybe_samples) {
    std::cerr << "ERROR: Malformed SPD file: " << files[0].first << std::endl;
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
  if (absl::holds_alternative<std::vector<std::pair<std::string, std::string>>>(
          parameter.data)) {
    return Match(absl::get<std::vector<std::pair<std::string, std::string>>>(
        parameter.data));
  } else {
    return Match(
        absl::get<std::pair<std::vector<std::string>, std::vector<float_t>>>(
            parameter.data));
  }
}

void SpectrumMatcher::Match(ParameterData& data) {
  if (absl::holds_alternative<ColorParameter>(data)) {
    m_value = Match(absl::get<ColorParameter>(data));
  } else {
    m_value = Match(absl::get<SpectrumParameter>(data));
  }
}

}  // namespace iris