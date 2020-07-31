#ifndef _SRC_PARAM_MATCHER_REFLECTOR_TEXTURE_
#define _SRC_PARAM_MATCHER_REFLECTOR_TEXTURE_

#include "src/common/named_texture_manager.h"
#include "src/common/parameter_matcher.h"
#include "src/common/pointer_types.h"
#include "src/common/spectrum_manager.h"
#include "src/common/texture_manager.h"

namespace iris {

class ReflectorTextureMatcher : public ParameterMatcher {
 public:
  ReflectorTextureMatcher(absl::string_view parameter_name, bool required,
                          const NamedTextureManager& named_texture_manager,
                          TextureManager& texture_manager,
                          SpectrumManager& spectrum_manager,
                          ReflectorTexture default_value);

  const ReflectorTexture& Get() const;

  static ReflectorTextureMatcher FromUniformReflectance(
      absl::string_view parameter_name, bool required,
      const NamedTextureManager& named_texture_manager,
      TextureManager& texture_manager, SpectrumManager& spectrum_manager,
      float_t default_reflectance);

 protected:
  void Match(ParameterData& data) final;

 private:
  ReflectorTexture Match(const FloatParameter& parameter);
  ReflectorTexture Match(const ColorParameter& parameter);
  ReflectorTexture Match(const SpectrumParameter& parameter);
  ReflectorTexture Match(const TextureParameter& parameter);

  ReflectorTexture Match(
      const std::vector<std::pair<std::string, std::string>>& files);
  ReflectorTexture Match(
      const std::pair<std::vector<std::string>, std::vector<float_t>>& samples);

  static bool ValidateFloat(float_t value);

 private:
  const NamedTextureManager& m_named_texture_manager;
  TextureManager& m_texture_manager;
  SpectrumManager& m_spectrum_manager;
  ReflectorTexture m_value;
};

}  // namespace iris

#endif  // _SRC_PARAM_MATCHER_REFLECTOR_TEXTURE_