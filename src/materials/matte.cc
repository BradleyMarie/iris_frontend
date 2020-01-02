#include "src/materials/matte.h"

#include <iostream>

#include "absl/strings/str_join.h"  // TODO: Move
#include "iris_physx_toolkit/constant_texture.h"
#include "iris_physx_toolkit/matte_material.h"
#include "iris_physx_toolkit/uniform_reflector.h"
#include "src/common/error.h"
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
static const float_t kMatteMaterialDefaultReflectance = (float_t)0.5;

}  // namespace

MaterialResult ParseMatte(const char* base_type_name, const char* type_name,
                          Tokenizer& tokenizer,
                          const ColorExtrapolator& color_extrapolator,
                          const TextureManager& texture_manager) {
  Reflector reflectance;
  ISTATUS status = UniformReflectorAllocate(
      kMatteMaterialDefaultReflectance, reflectance.release_and_get_address());
  SuccessOrOOM(status);

  SingleReflectorMatcher kd(base_type_name, type_name, "Kd", false,
                            reflectance);
  MatchParameters<1>(base_type_name, type_name, tokenizer, {&kd});

  ReflectorTexture diffuse;
  status = ConstantReflectorTextureAllocate(kd.Get().get(),
                                            diffuse.release_and_get_address());
  SuccessOrOOM(status);

  Material result;
  status =
      MatteMaterialAllocate(diffuse.get(), result.release_and_get_address());
  SuccessOrOOM(status);

  return std::make_pair(result, std::set<Reflector>({kd.Get()}));
}

}  // namespace iris