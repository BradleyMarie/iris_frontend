#include "src/area_lights/diffuse.h"

#include "iris_physx_toolkit/constant_emissive_material.h"
#include "src/common/error.h"
#include "src/param_matchers/matcher.h"
#include "src/param_matchers/single.h"
#include "src/param_matchers/spectrum_single.h"

namespace iris {
namespace {

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