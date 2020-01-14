#include "src/param_matchers/float_texture.h"

namespace iris {
namespace {

bool ValidateFloatImpl(bool inclusive, float_t minimum, float_t maximum,
                       float_t value) {
  assert(!std::isnan(minimum));
  assert(!std::isnan(maximum));
  assert(!inclusive || std::isfinite(minimum));
  assert(!inclusive || std::isfinite(maximum));
  assert((inclusive && minimum <= maximum) ||
         (!inclusive && minimum < maximum));

  bool valid;
  if (inclusive) {
    valid = minimum <= value && value <= maximum;
  } else {
    valid = minimum < value && value < maximum;
  }

  return valid;
}

}  // namespace

const size_t FloatTextureMatcher::m_variant_indices[2] = {
    GetIndex<FloatParameter, ParameterData>(),
    GetIndex<TextureParameter, ParameterData>()};

FloatTextureMatcher FloatTextureMatcher::FromValue(
    const char* base_type_name, const char* type_name,
    const char* parameter_name, bool required, bool inclusive, float_t minimum,
    float_t maximum, const NamedTextureManager& named_texture_manager,
    TextureManager& texture_manager, float_t default_value) {
  assert(ValidateFloatImpl(inclusive, minimum, maximum, default_value));
  return FloatTextureMatcher(
      base_type_name, type_name, parameter_name, required, inclusive, minimum,
      maximum, named_texture_manager, texture_manager,
      std::move(texture_manager.AllocateConstantFloatTexture(default_value)));
}

bool FloatTextureMatcher::ValidateFloat(float_t value) const {
  return ValidateFloatImpl(m_inclusive, m_minimum, m_maximum, value);
}

FloatTexture FloatTextureMatcher::Match(const FloatParameter& parameter) const {
  if (parameter.data.size() != 1) {
    NumberOfElementsError();
  }
  if (!ValidateFloat(parameter.data[0])) {
    ElementRangeError();
  }
  return m_texture_manager.AllocateConstantFloatTexture(parameter.data[0]);
}

FloatTexture FloatTextureMatcher::Match(
    const TextureParameter& parameter) const {
  if (parameter.data.size() != 1) {
    NumberOfElementsError();
  }
  return m_named_texture_manager.GetFloatTexture(parameter.data[0]);
}

void FloatTextureMatcher::Match(ParameterData& data) {
  if (absl::holds_alternative<FloatParameter>(data)) {
    m_value = Match(absl::get<FloatParameter>(data));
  } else {
    m_value = Match(absl::get<TextureParameter>(data));
  }
}

}  // namespace iris