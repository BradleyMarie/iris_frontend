#ifndef _SRC_PARAM_MATCHER_REFLECTOR_TEXTURE_
#define _SRC_PARAM_MATCHER_REFLECTOR_TEXTURE_

#include "src/common/error.h"
#include "src/common/named_texture_manager.h"
#include "src/common/pointer_types.h"
#include "src/common/spectrum_manager.h"
#include "src/common/texture_manager.h"
#include "src/param_matchers/matcher.h"

namespace iris {

class ReflectorTextureMatcher : public ParamMatcher {
 public:
  ReflectorTextureMatcher(const char* base_type_name, const char* type_name,
                          const char* parameter_name, bool required,
                          const NamedTextureManager& named_texture_manager,
                          TextureManager& texture_manager,
                          SpectrumManager& spectrum_manager,
                          ReflectorTexture default_value)
      : ParamMatcher(base_type_name, type_name, parameter_name, required,
                     m_variant_indices, 5),
        m_named_texture_manager(named_texture_manager),
        m_texture_manager(texture_manager),
        m_spectrum_manager(spectrum_manager),
        m_value(std::move(default_value)) {}

  const ReflectorTexture& Get() { return m_value; }

  static ReflectorTextureMatcher FromUniformReflectance(
      const char* base_type_name, const char* type_name,
      const char* parameter_name, bool required,
      const NamedTextureManager& named_texture_manager,
      TextureManager& texture_manager, SpectrumManager& spectrum_manager,
      float_t default_reflectance);

 protected:
  void Match(ParameterData& data) final;

 private:
  ReflectorTexture Match(const FloatParameter& parameter);
  ReflectorTexture Match(const RgbParameter& parameter);
  ReflectorTexture Match(const SpectrumParameter& parameter);
  ReflectorTexture Match(const TextureParameter& parameter);
  ReflectorTexture Match(const XyzParameter& parameter);

  ReflectorTexture Match(const std::vector<std::string>& files);
  ReflectorTexture Match(
      const std::pair<std::vector<std::string>, std::vector<float_t>>& samples);

  static bool ValidateFloat(float_t value);

 private:
  static const size_t m_variant_indices[5];
  const NamedTextureManager& m_named_texture_manager;
  TextureManager& m_texture_manager;
  SpectrumManager& m_spectrum_manager;
  ReflectorTexture m_value;
};

}  // namespace iris

#endif  // _SRC_PARAM_MATCHER_REFLECTOR_TEXTURE_