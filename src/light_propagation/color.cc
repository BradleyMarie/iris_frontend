#include "src/light_propagation/color.h"

#include <iostream>

#include "iris_physx_toolkit/color_spectra.h"
#include "src/common/error.h"
#include "src/param_matchers/single.h"

namespace iris {
namespace {

static const char* kColorLightPropagationColorSpace = "linearsrgb";

}  // namespace

LightPropagationResult ParseColor(const char* base_type_name,
                                  const char* type_name, Tokenizer& tokenizer) {
  SingleStringMatcher colorspace(base_type_name, type_name, "colorspace", false,
                                 kColorLightPropagationColorSpace);
  MatchParameters(base_type_name, type_name, tokenizer, {&colorspace});

  COLOR_SPACE color_space;
  if (colorspace.Get() == "xyz") {
    color_space = COLOR_SPACE_XYZ;
  } else if (colorspace.Get() == "linearsrgb") {
    color_space = COLOR_SPACE_LINEAR_SRGB;
  } else {
    std::cerr << "ERROR: Unsupported colorspace for color "
                 "LightPropagation: "
              << colorspace.Get() << std::endl;
    exit(EXIT_FAILURE);
  }

  return [=](const ColorIntegrator& color_integrator) {
    ColorIntegrator final_color_integrator;
    ISTATUS status = ColorColorIntegratorAllocate(
        color_space, final_color_integrator.release_and_get_address());
    SuccessOrOOM(status);

    ColorExtrapolator extrapolator;
    status = ColorColorExtrapolatorAllocate(
        color_space, extrapolator.release_and_get_address());
    SuccessOrOOM(status);

    return std::make_pair(
        SpectrumManager(std::move(extrapolator), color_integrator),
        final_color_integrator);
  };
}

}  // namespace iris