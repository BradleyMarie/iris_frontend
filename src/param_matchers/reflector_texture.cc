#include "src/param_matchers/reflector_texture.h"

#include <iostream>

#include "absl/strings/str_join.h"
#include "src/param_matchers/spd_file.h"

namespace iris {
namespace {

ReflectorTexture ReflectorFromUniformReflectance(
    TextureManager& texture_manager, SpectrumManager& spectrum_manager,
    float_t reflectance) {
  Reflector reflector =
      spectrum_manager.AllocateUniformReflector(reflectance).value();
  return texture_manager.AllocateConstantReflectorTexture(reflector);
}

}  // namespace

const size_t ReflectorTextureMatcher::m_variant_indices[4] = {
    GetIndex<FloatParameter, ParameterData>(),
    GetIndex<ColorParameter, ParameterData>(),
    GetIndex<SpectrumParameter, ParameterData>(),
    GetIndex<TextureParameter, ParameterData>()};

ReflectorTextureMatcher ReflectorTextureMatcher::FromUniformReflectance(
    const char* base_type_name, const char* type_name,
    const char* parameter_name, bool required,
    const Tokenizer& tokenizer,
    const NamedTextureManager& named_texture_manager,
    TextureManager& texture_manager, SpectrumManager& spectrum_manager,
    float_t default_reflectance) {
  assert(ValidateFloat(default_reflectance));
  return ReflectorTextureMatcher(
      base_type_name, type_name, parameter_name, required, tokenizer,
      named_texture_manager, texture_manager, spectrum_manager,
      std::move(ReflectorFromUniformReflectance(
          texture_manager, spectrum_manager, default_reflectance)));
}

bool ReflectorTextureMatcher::ValidateFloat(float_t value) {
  if (!std::isfinite(value) || value < (float_t)0.0 || (float_t)1.0 < value) {
    return false;
  }

  return true;
}

ReflectorTexture ReflectorTextureMatcher::Match(
    const FloatParameter& parameter) {
  if (parameter.data.size() != 1) {
    NumberOfElementsError();
  }
  if (!ValidateFloat(parameter.data[0])) {
    ElementRangeError();
  }
  return ReflectorFromUniformReflectance(m_texture_manager, m_spectrum_manager,
                                         parameter.data[0]);
}

ReflectorTexture ReflectorTextureMatcher::Match(
    const ColorParameter& parameter) {
  if (parameter.data.size() != 1) {
    NumberOfElementsError();
  }
  if (!ValidateFloat(parameter.data[0].values[0]) ||
      !ValidateFloat(parameter.data[0].values[1]) ||
      !ValidateFloat(parameter.data[0].values[2])) {
    ElementRangeError();
  }
  Reflector reflector =
      m_spectrum_manager.AllocateColorReflector(parameter.data[0]).value();
  return m_texture_manager.AllocateConstantReflectorTexture(reflector);
}

ReflectorTexture ReflectorTextureMatcher::Match(
    const std::vector<std::string>& files) {
  if (files.size() != 1) {
    NumberOfElementsError();
  }
  auto maybe_samples = ReadSpdFile(files[0], m_tokenizer.ResolvePath(files[0]));
  if (!maybe_samples) {
    std::cerr << "ERROR: Malformed SPD file: " << files[0] << std::endl;
    exit(EXIT_FAILURE);
  }
  auto maybe_reflector =
      m_spectrum_manager.AllocateInterpolatedReflector(*maybe_samples);
  if (!maybe_samples) {
    std::cerr << "ERROR: Malformed reflection SPD file: " << files[0]
              << std::endl;
    exit(EXIT_FAILURE);
  }
  return m_texture_manager.AllocateConstantReflectorTexture(*maybe_reflector);
}

ReflectorTexture ReflectorTextureMatcher::Match(
    const std::pair<std::vector<std::string>, std::vector<float_t>>& samples) {
  if (samples.second.size() % 2 != 0) {
    NumberOfElementsError();
  }
  auto maybe_reflector =
      m_spectrum_manager.AllocateInterpolatedReflector(samples.second);
  if (!maybe_reflector) {
    std::cerr
        << "ERROR: Could not construct a reflection spectrum from values ("
        << absl::StrJoin(samples.first, ", ") << ")" << std::endl;
    exit(EXIT_FAILURE);
  }
  return m_texture_manager.AllocateConstantReflectorTexture(*maybe_reflector);
}

ReflectorTexture ReflectorTextureMatcher::Match(
    const SpectrumParameter& parameter) {
  if (absl::holds_alternative<std::vector<std::string>>(parameter.data)) {
    return Match(absl::get<std::vector<std::string>>(parameter.data));
  } else {
    return Match(
        absl::get<std::pair<std::vector<std::string>, std::vector<float_t>>>(
            parameter.data));
  }
}

ReflectorTexture ReflectorTextureMatcher::Match(
    const TextureParameter& parameter) {
  if (parameter.data.size() != 1) {
    NumberOfElementsError();
  }
  return m_named_texture_manager.GetReflectorTexture(parameter.data[0]);
}

void ReflectorTextureMatcher::Match(ParameterData& data) {
  if (absl::holds_alternative<FloatParameter>(data)) {
    m_value = Match(absl::get<FloatParameter>(data));
  } else if (absl::holds_alternative<ColorParameter>(data)) {
    m_value = Match(absl::get<ColorParameter>(data));
  } else if (absl::holds_alternative<SpectrumParameter>(data)) {
    m_value = Match(absl::get<SpectrumParameter>(data));
  } else {
    m_value = Match(absl::get<TextureParameter>(data));
  }
}

}  // namespace iris