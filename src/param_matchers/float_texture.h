#ifndef _SRC_PARAM_MATCHER_FLOAT_TEXTURE_
#define _SRC_PARAM_MATCHER_FLOAT_TEXTURE_

#include "src/common/named_texture_manager.h"
#include "src/common/parameter_matcher.h"
#include "src/common/pointer_types.h"
#include "src/common/texture_manager.h"

namespace iris {

class FloatTextureMatcher : public ParameterMatcher {
 public:
  FloatTextureMatcher(absl::string_view parameter_name, bool required,
                      bool inclusive, float_t minimum, float_t maximum,
                      const NamedTextureManager& named_texture_manager,
                      TextureManager& texture_manager,
                      FloatTexture default_value);
  const FloatTexture& Get() const;

  static FloatTextureMatcher FromValue(
      absl::string_view parameter_name, bool required, bool inclusive,
      float_t minimum, float_t maximum,
      const NamedTextureManager& named_texture_manager,
      TextureManager& texture_manager, float_t default_value);

 protected:
  void Match(ParameterData& data) final;

 private:
  FloatTexture Match(const FloatParameter& parameter) const;
  FloatTexture Match(const TextureParameter& parameter) const;

  bool ValidateFloat(float_t value) const;

 private:
  const NamedTextureManager& m_named_texture_manager;
  TextureManager& m_texture_manager;
  FloatTexture m_value;
  float_t m_minimum;
  float_t m_maximum;
  bool m_inclusive;
};

}  // namespace iris

#endif  // _SRC_PARAM_MATCHER_FLOAT_TEXTURE_