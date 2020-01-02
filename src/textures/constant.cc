#include "src/textures/constant.h"

#include <iostream>  // TODO: Remove

#include "absl/strings/str_join.h"  // TODO: Move
#include "iris_physx_toolkit/constant_texture.h"
#include "iris_physx_toolkit/uniform_reflector.h"
#include "src/common/error.h"
#include "src/param_matchers/float_single.h"  // TODO: Remove
#include "src/param_matchers/matcher.h"

namespace iris {
namespace {

// TODO: Move
class SingleReflectorMatcher : public ParamMatcher {
 public:
  SingleReflectorMatcher(const char* base_type_name, const char* type_name,
                         const char* parameter_name, bool required,
                         Reflector default_value)
      : ParamMatcher(base_type_name, type_name, parameter_name, required,
                     &m_variant_index, 1),
        m_value(default_value) {}
  const Reflector& Get() { return m_value; }

 private:
  void Match(ParameterData& data) final {
    if (absl::get<SpectrumParameter>(data).data.size() != 1) {
      NumberOfElementsError();
    }
    if (!absl::get<SpectrumParameter>(data).data[0].second.get()) {
      std::cerr
          << "ERROR: Could not construct a reflection spectrum from values ("
          << absl::StrJoin(absl::get<SpectrumParameter>(data).values[0], ", ")
          << ")" << std::endl;
      exit(EXIT_FAILURE);
    }
    m_value = absl::get<SpectrumParameter>(data).data[0].second;
  }

 private:
  static const size_t m_variant_index;
  Reflector m_value;
};

const size_t SingleReflectorMatcher::m_variant_index =
    GetIndex<SpectrumParameter, ParameterData>();
static const float_t kConstantTextureDefaultValue = (float_t)1.0;

}  // namespace

std::pair<ReflectorTexture, std::set<Reflector>> ParseConstantReflector(
    const char* base_type_name, const char* type_name, Tokenizer& tokenizer,
    const ColorExtrapolator& color_extrapolator,
    const TextureManager& texture_manager) {
  Reflector reflectance;
  ISTATUS status = UniformReflectorAllocate(
      kConstantTextureDefaultValue, reflectance.release_and_get_address());
  SuccessOrOOM(status);

  SingleReflectorMatcher value(base_type_name, type_name, "value", false,
                               reflectance);
  MatchParameters<1>(base_type_name, type_name, tokenizer, {&value});

  ReflectorTexture texture;
  status = ConstantReflectorTextureAllocate(value.Get().get(),
                                            texture.release_and_get_address());
  SuccessOrOOM(status);

  return std::make_pair(texture, std::set<Reflector>({value.Get()}));
}

FloatTexture ParseConstantFloat(const char* base_type_name,
                                const char* type_name, Tokenizer& tokenizer,
                                const ColorExtrapolator& color_extrapolator,
                                const TextureManager& texture_manager) {
  SingleFloatMatcher value(base_type_name, type_name, "value", false, true,
                           (float_t)0.0, (float_t)1.0,
                           kConstantTextureDefaultValue);

  FloatTexture texture;
  ISTATUS status = ConstantFloatTextureAllocate(
      value.Get(), texture.release_and_get_address());
  SuccessOrOOM(status);

  return texture;
}

}  // namespace iris