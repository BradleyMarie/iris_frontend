#include "src/area_lights/diffuse.h"

#include "iris_physx_toolkit/constant_emissive_material.h"
#include "src/common/error.h"
#include "src/param_matchers/matcher.h"
#include "src/param_matchers/single.h"
#include "src/param_matchers/spectrum.h"

namespace iris {
namespace {

static const bool kDiffuseAreaLightDefaultTwoSided = false;
static const std::array<float_t, 3> kDiffuseAreaLightDefaultL = {
    (float_t)1.0, (float_t)1.0, (float_t)1.0};

}  // namespace

AreaLightResult ParseDiffuse(Parameters& parameters,
                             SpectrumManager& spectrum_manager) {
  SingleBoolMatcher twosided("twosided", false,
                             kDiffuseAreaLightDefaultTwoSided);
  SpectrumMatcher spectrum = SpectrumMatcher::FromRgb(
      "L", false, spectrum_manager, kDiffuseAreaLightDefaultL);
  parameters.Match(twosided, spectrum);

  EmissiveMaterial front_emissive_material, back_emissive_material;
  ISTATUS status = ConstantEmissiveMaterialAllocate(
      (PSPECTRUM)spectrum.Get().get(),
      front_emissive_material.release_and_get_address());
  SuccessOrOOM(status);

  if (twosided.Get()) {
    back_emissive_material = front_emissive_material;
  }

  return std::make_pair(front_emissive_material, back_emissive_material);
}

}  // namespace iris