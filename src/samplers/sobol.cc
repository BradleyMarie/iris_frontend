#include "src/samplers/sobol.h"

#include "iris_camera_toolkit/sobol_image_sampler.h"
#include "src/common/error.h"
#include "src/param_matchers/integral_single.h"

namespace iris {
namespace {

static const uint16_t kSobolSamplerDefaultPixelSamples = 16;

}  // namespace

Sampler ParseSobol(Parameters& parameters) {
  NonZeroSingleUInt16Matcher pixelsamples("pixelsamples", false,
                                          kSobolSamplerDefaultPixelSamples);
  parameters.Match(pixelsamples);

  Sampler result;
  ISTATUS status = SobolImageSamplerAllocate(pixelsamples.Get(),
                                             result.release_and_get_address());
  SuccessOrOOM(status);

  return result;
}

}  // namespace iris