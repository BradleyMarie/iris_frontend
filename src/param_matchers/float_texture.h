#ifndef _SRC_PARAM_MATCHER_FLOAT_TEXTURE_
#define _SRC_PARAM_MATCHER_FLOAT_TEXTURE_

#include "src/common/error.h"
#include "src/common/pointer_types.h"
#include "src/common/texture_manager.h"
#include "src/param_matchers/matcher.h"

namespace iris {

class FloatTextureMatcher : public ParamMatcher {
 public:
  FloatTextureMatcher(const char* base_type_name, const char* type_name,
                      const char* parameter_name, bool required, bool inclusive,
                      float_t minimum, float_t maximum,
                      TextureManager& texture_manager,
                      FloatTexture default_value)
      : ParamMatcher(base_type_name, type_name, parameter_name, required,
                     m_variant_indices, 3),
        m_texture_manager(texture_manager),
        m_value(std::move(default_value)),
        m_minimum(minimum),
        m_maximum(maximum),
        m_inclusive(inclusive) {
    assert(!inclusive || std::isfinite(minimum));
    assert(!inclusive || std::isfinite(maximum));
    assert((inclusive && minimum <= maximum) ||
           (!inclusive && minimum < maximum));
  }
  const FloatTexture& Get() { return m_value; }

  static FloatTextureMatcher FromValue(const char* base_type_name,
                                       const char* type_name,
                                       const char* parameter_name,
                                       bool required, bool inclusive,
                                       float_t minimum, float_t maximum,
                                       TextureManager& texture_manager,
                                       float_t default_value);

 protected:
  void Match(ParameterData& data) final;

 private:
  FloatTexture Match(const FloatParameter& parameter) const;
  FloatTexture Match(const TextureParameter& parameter,
                     TextureManager& texture_manager) const;

  bool ValidateFloat(float_t value) const;

 private:
  static const size_t m_variant_indices[2];
  TextureManager& m_texture_manager;
  FloatTexture m_value;
  float_t m_minimum;
  float_t m_maximum;
  bool m_inclusive;
};

}  // namespace iris

#endif  // _SRC_PARAM_MATCHER_FLOAT_TEXTURE_