#include "src/light_propagation/spectrum.h"

#include <iostream>

#include "iris_physx_toolkit/smits_color_extrapolator.h"
#include "src/common/error.h"
#include "src/param_matchers/single.h"

namespace iris {
namespace {

static const char* kSpectrumLightPropagationColorExtrapolator = "smits";
static const COLOR_SPACE kSpectrumLightPropagationColorSpace =
    COLOR_SPACE_LINEAR_SRGB;

}  // namespace

LightPropagationResult ParseSpectrum(Parameters& parameters) {
  SingleStringMatcher colorextrapolator(
      "colorextrapolator", false, kSpectrumLightPropagationColorExtrapolator);
  parameters.Match(colorextrapolator);

  if (colorextrapolator.Get() != "smits") {
    std::cerr << "ERROR: Unsupported colorextrapolator for "
              << parameters.Type() << " " << parameters.BaseType() << ": "
              << colorextrapolator.Get() << std::endl;
    exit(EXIT_FAILURE);
  }

  return [=](const ColorIntegrator& color_integrator) {
    std::vector<float_t> wavelengths;
    for (size_t lambda = 360; lambda <= 830; lambda++) {
      wavelengths.push_back(lambda);
    }

    ColorExtrapolator extrapolator;
    ISTATUS status =
        SmitsColorExtrapolatorAllocate(wavelengths.data(), wavelengths.size(),
                                       extrapolator.release_and_get_address());
    SuccessOrOOM(status);

    return std::make_pair(SpectrumManager(std::move(extrapolator),
                                          kSpectrumLightPropagationColorSpace),
                          color_integrator);
  };
}

}  // namespace iris