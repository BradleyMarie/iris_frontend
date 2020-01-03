#include "src/param_matchers/reflector_texture.h"

#include <iostream>

#include "absl/strings/str_join.h"
#include "iris_physx_toolkit/constant_texture.h"
#include "iris_physx_toolkit/uniform_reflector.h"

namespace iris {
namespace {

std::pair<ReflectorTexture, std::set<Reflector>>
ReflectorFromUniformReflectance(float_t reflectance) {
  Reflector reflector;
  ISTATUS status = UniformReflectorAllocate(
      reflectance, reflector.release_and_get_address());
  SuccessOrOOM(status);
  ReflectorTexture result;
  status = ConstantReflectorTextureAllocate(reflector.get(),
                                            result.release_and_get_address());
  SuccessOrOOM(status);
  return std::make_pair(std::move(result), std::set<Reflector>({reflector}));
}

std::pair<ReflectorTexture, std::set<Reflector>> ReflectorFromRgb(
    const ColorExtrapolator& color_extrapolator,
    const std::array<float_t, 3>& rgb) {
  Reflector reflector;
  ISTATUS status = RgbInterpolatorAllocateReflector(
      color_extrapolator.get(), rgb[0], rgb[1], rgb[2],
      reflector.release_and_get_address());
  SuccessOrOOM(status);
  ReflectorTexture result;
  status = ConstantReflectorTextureAllocate(reflector.get(),
                                            result.release_and_get_address());
  SuccessOrOOM(status);
  return std::make_pair(std::move(result), std::set<Reflector>({reflector}));
}

}  // namespace

const size_t ReflectorTextureMatcher::m_variant_indices[5] = {
    GetIndex<FloatParameter, ParameterData>(),
    GetIndex<RgbParameter, ParameterData>(),
    GetIndex<SpectrumParameter, ParameterData>(),
    GetIndex<TextureParameter, ParameterData>(),
    GetIndex<XyzParameter, ParameterData>()};

ReflectorTextureMatcher ReflectorTextureMatcher::FromUniformReflectance(
    const char* base_type_name, const char* type_name,
    const char* parameter_name, bool required,
    const TextureManager& texture_manager,
    const ColorExtrapolator& color_extrapolator, float_t default_reflectance) {
  return ReflectorTextureMatcher(
      base_type_name, type_name, parameter_name, required, texture_manager,
      color_extrapolator,
      std::move(ReflectorFromUniformReflectance(default_reflectance)));
}

std::pair<ReflectorTexture, std::set<Reflector>> ReflectorTextureMatcher::Match(
    const FloatParameter& parameter) const {
  if (parameter.data.size() != 1) {
    NumberOfElementsError();
  }
  return ReflectorFromUniformReflectance(parameter.data[0]);
}

std::pair<ReflectorTexture, std::set<Reflector>> ReflectorTextureMatcher::Match(
    const RgbParameter& parameter) const {
  if (parameter.data.size() != 1) {
    NumberOfElementsError();
  }
  return ReflectorFromRgb(m_color_extrapolator, parameter.data[0]);
}

std::pair<ReflectorTexture, std::set<Reflector>> ReflectorTextureMatcher::Match(
    const SpectrumParameter& parameter) const {
  if (parameter.data.size() != 1) {
    NumberOfElementsError();
  }
  if (!parameter.data[0].second.get()) {
    std::cerr
        << "ERROR: Could not construct a reflection spectrum from values ("
        << absl::StrJoin(parameter.values[0], ", ") << ")" << std::endl;
    exit(EXIT_FAILURE);
  }
  ReflectorTexture result;
  ISTATUS status = ConstantReflectorTextureAllocate(
      parameter.data[0].second.get(), result.release_and_get_address());
  SuccessOrOOM(status);
  return std::make_pair(std::move(result),
                        std::set<Reflector>({parameter.data[0].second}));
}

std::pair<ReflectorTexture, std::set<Reflector>> ReflectorTextureMatcher::Match(
    const TextureParameter& parameter,
    const TextureManager& texture_manager) const {
  if (parameter.data.size() != 1) {
    NumberOfElementsError();
  }
  return texture_manager.GetReflectorTexture(parameter.data[0]);
}

std::pair<ReflectorTexture, std::set<Reflector>> ReflectorTextureMatcher::Match(
    const XyzParameter& parameter) const {
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

  return ReflectorFromRgb(m_color_extrapolator, {r, g, b});
}

void ReflectorTextureMatcher::Match(ParameterData& data) {
  if (absl::holds_alternative<FloatParameter>(data)) {
    m_value = Match(absl::get<FloatParameter>(data));
  } else if (absl::holds_alternative<RgbParameter>(data)) {
    m_value = Match(absl::get<RgbParameter>(data));
  } else if (absl::holds_alternative<SpectrumParameter>(data)) {
    m_value = Match(absl::get<SpectrumParameter>(data));
  } else if (absl::holds_alternative<TextureParameter>(data)) {
    m_value = Match(absl::get<TextureParameter>(data), m_texture_manager);
  } else {
    m_value = Match(absl::get<XyzParameter>(data));
  }
}

}  // namespace iris