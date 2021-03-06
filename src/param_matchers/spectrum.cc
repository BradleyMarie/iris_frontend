#include "src/param_matchers/spectrum.h"

#include <iostream>

#include "absl/strings/str_join.h"
#include "src/param_matchers/spd_file.h"

namespace iris {
namespace {

const size_t variant_indices[3] = {
    GetIndex<ColorParameter, ParameterData>(),
    GetIndex<SpectrumParameter, ParameterData>(),
    GetIndex<UnspacedColorParameter, ParameterData>()};

}  // namespace

SpectrumMatcher::SpectrumMatcher(absl::string_view parameter_name,
                                 bool required,
                                 SpectrumManager& spectrum_manager,
                                 Spectrum default_value)
    : ParameterMatcher(parameter_name, required, variant_indices),
      m_spectrum_manager(spectrum_manager),
      m_value(std::move(default_value)) {}

const Spectrum& SpectrumMatcher::Get() const { return m_value; }

SpectrumMatcher SpectrumMatcher::FromRgb(
    absl::string_view parameter_name, bool required,
    SpectrumManager& spectrum_manager,
    const std::array<float_t, 3>& default_rgb) {
  return SpectrumMatcher(
      parameter_name, required, spectrum_manager,
      spectrum_manager.AllocateColorSpectrum(default_rgb).value());
}

Spectrum SpectrumMatcher::Match(const ColorParameter& parameter) {
  if (parameter.data.size() != 1) {
    NumberOfElementsError();
  }
  return m_spectrum_manager.AllocateColorSpectrum(parameter.data[0]).value();
}

Spectrum SpectrumMatcher::Match(const UnspacedColorParameter& parameter) {
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
  } if (absl::holds_alternative<UnspacedColorParameter>(data)) {
    m_value = Match(absl::get<UnspacedColorParameter>(data));
  } else {
    m_value = Match(absl::get<SpectrumParameter>(data));
  }
}

}  // namespace iris