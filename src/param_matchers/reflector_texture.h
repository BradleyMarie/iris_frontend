#ifndef _SRC_PARAM_MATCHER_REFLECTOR_TEXTURE_
#define _SRC_PARAM_MATCHER_REFLECTOR_TEXTURE_

#include "src/common/error.h"
#include "src/common/pointer_types.h"
#include "src/common/texture_manager.h"
#include "src/param_matchers/matcher.h"

namespace iris {

class ReflectorTextureMatcher : public ParamMatcher {
 public:
  ReflectorTextureMatcher(
      const char* base_type_name, const char* type_name,
      const char* parameter_name, bool required, bool inclusive,
      float_t minimum, float_t maximum, const TextureManager& texture_manager,
      const ColorExtrapolator& color_extrapolator,
      std::pair<ReflectorTexture, std::set<Reflector>> default_value)
      : ParamMatcher(base_type_name, type_name, parameter_name, required,
                     m_variant_indices, 3),
        m_texture_manager(texture_manager),
        m_color_extrapolator(color_extrapolator),
        m_value(std::move(default_value)),
        m_minimum(minimum),
        m_maximum(maximum),
        m_inclusive(inclusive) {
    assert(!inclusive || std::isfinite(minimum));
    assert(!inclusive || std::isfinite(maximum));
    assert(inclusive || minimum < maximum);
  }
  const std::pair<ReflectorTexture, std::set<Reflector>>& Get() {
    return m_value;
  }

  static ReflectorTextureMatcher FromUniformReflectance(
      const char* base_type_name, const char* type_name,
      const char* parameter_name, bool required, bool inclusive,
      float_t minimum, float_t maximum, const TextureManager& texture_manager,
      const ColorExtrapolator& color_extrapolator, float_t default_reflectance);

 private:
  void Match(ParameterData& data) final;

  std::pair<ReflectorTexture, std::set<Reflector>> Match(
      const FloatParameter& parameter) const;
  std::pair<ReflectorTexture, std::set<Reflector>> Match(
      const RgbParameter& parameter) const;
  std::pair<ReflectorTexture, std::set<Reflector>> Match(
      const SpectrumParameter& parameter) const;
  std::pair<ReflectorTexture, std::set<Reflector>> Match(
      const TextureParameter& parameter,
      const TextureManager& texture_manager) const;
  std::pair<ReflectorTexture, std::set<Reflector>> Match(
      const XyzParameter& parameter) const;

 private:
  static const size_t m_variant_indices[5];
  const TextureManager& m_texture_manager;
  const ColorExtrapolator& m_color_extrapolator;
  std::pair<ReflectorTexture, std::set<Reflector>> m_value;
  float_t m_minimum;
  float_t m_maximum;
  bool m_inclusive;
};

}  // namespace iris

#endif  // _SRC_PARAM_MATCHER_REFLECTOR_TEXTURE_