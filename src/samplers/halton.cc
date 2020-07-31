#include "src/samplers/halton.h"

#include "iris_camera_toolkit/halton_image_sampler.h"
#include "src/common/error.h"
#include "src/param_matchers/integral_single.h"

namespace iris {
namespace {

static const uint16_t kHaltonSamplerDefaultPixelSamples = 16;

}  // namespace

Sampler ParseHalton(Parameters& parameters) {
  NonZeroSingleUInt16Matcher pixelsamples("pixelsamples", false,
                                          kHaltonSamplerDefaultPixelSamples);
  parameters.Match(pixelsamples);

  Sampler result;
  ISTATUS status = HaltonImageSamplerAllocate(pixelsamples.Get(),
                                              result.release_and_get_address());
  SuccessOrOOM(status);

  return result;
}

}  // namespace iris