#include "src/area_lights/diffuse.h"

#include "iris_physx_toolkit/constant_emissive_material.h"
#include "src/param_matcher.h"

#include <iostream>

namespace iris {
namespace {

// TODO: Move
class SingleSpectrumMatcher : public ParamMatcher {
 public:
  SingleSpectrumMatcher(const char* base_type_name, const char* type_name,
                        const char* parameter_name, Spectrum default_value)
      : ParamMatcher(base_type_name, type_name, parameter_name,
                     GetIndex<SpectrumParameter>()),
        m_value(default_value) {}
  const Spectrum& Get() { return m_value; }

 private:
  void Match(const ParameterData& data) final {
    if (absl::get<SpectrumParameter>(data).data.size() != 1) {
      NumberOfElementsError();
    }
    m_value = absl::get<SpectrumParameter>(data).data[0].first;
  }

 private:
  Spectrum m_value;
};

static const bool kDiffuseAreaLightDefaultTwoSided = false;
static const Spectrum kDiffuseAreaLightDefaultL;  // TODO: initialize

}  // namespace

AreaLightResult ParseDiffuse(const char* base_type_name, const char* type_name,
                             Tokenizer& tokenizer) {
  SingleBoolMatcher twosided(base_type_name, type_name, "twosided",
                             kDiffuseAreaLightDefaultTwoSided);
  SingleSpectrumMatcher spectrum(base_type_name, type_name, "L",
                                 kDiffuseAreaLightDefaultL);
  ParseAllParameter<2>(base_type_name, type_name, tokenizer,
                       {&twosided, &spectrum});

  EmissiveMaterial front_emissive_material, back_emissive_material;
  ISTATUS status = ConstantEmissiveMaterialAllocate(
      (PSPECTRUM)spectrum.Get().get(),
      front_emissive_material.release_and_get_address());
  switch (status) {
    case ISTATUS_ALLOCATION_FAILED:
      std::cerr << "ERROR: Allocation failed" << std::endl;
      exit(EXIT_FAILURE);
    default:
      assert(status == ISTATUS_SUCCESS);
  }

  if (twosided.Get()) {
    back_emissive_material = front_emissive_material;
  }

  return std::make_tuple(front_emissive_material, back_emissive_material,
                         std::set<Spectrum>({spectrum.Get()}));
}

}  // namespace iris