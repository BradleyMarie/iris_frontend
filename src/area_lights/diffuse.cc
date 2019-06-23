#include "src/area_lights/diffuse.h"

#include <iostream>

#include "iris_physx_toolkit/constant_emissive_material.h"
#include "src/common/error.h"
#include "src/param_matchers/matcher.h"
#include "src/param_matchers/single.h"

namespace iris {
namespace {

// TODO: Move
class SingleSpectrumMatcher : public ParamMatcher {
 public:
  SingleSpectrumMatcher(const char* base_type_name, const char* type_name,
                        const char* parameter_name, bool required,
                        Spectrum default_value)
      : ParamMatcher(base_type_name, type_name, parameter_name, required,
                     &m_variant_index, 1),
        m_value(default_value) {}
  const Spectrum& Get() { return m_value; }

 private:
  void Match(ParameterData& data) final {
    if (absl::get<SpectrumParameter>(data).data.size() != 1) {
      NumberOfElementsError();
    }
    m_value = absl::get<SpectrumParameter>(data).data[0].first;
  }

 private:
  static const size_t m_variant_index;
  Spectrum m_value;
};

const size_t SingleSpectrumMatcher::m_variant_index =
    GetIndex<SpectrumParameter, ParameterData>();

static const bool kDiffuseAreaLightDefaultTwoSided = false;
static const Spectrum kDiffuseAreaLightDefaultL;  // TODO: initialize

}  // namespace

AreaLightResult ParseDiffuse(const char* base_type_name, const char* type_name,
                             Tokenizer& tokenizer) {
  SingleBoolMatcher twosided(base_type_name, type_name, "twosided", false,
                             kDiffuseAreaLightDefaultTwoSided);
  SingleSpectrumMatcher spectrum(base_type_name, type_name, "L", false,
                                 kDiffuseAreaLightDefaultL);
  MatchParameters<2>(base_type_name, type_name, tokenizer,
                     {&twosided, &spectrum});

  EmissiveMaterial front_emissive_material, back_emissive_material;
  ISTATUS status = ConstantEmissiveMaterialAllocate(
      (PSPECTRUM)spectrum.Get().get(),
      front_emissive_material.release_and_get_address());
  SuccessOrOOM(status);

  if (twosided.Get()) {
    back_emissive_material = front_emissive_material;
  }

  return std::make_tuple(front_emissive_material, back_emissive_material,
                         std::set<Spectrum>({spectrum.Get()}));
}

}  // namespace iris