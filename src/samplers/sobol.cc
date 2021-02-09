#include "src/samplers/sobol.h"

#include "iris_advanced_toolkit/sobol_sequence.h"
#include "iris_camera_toolkit/low_discrepancy_image_sampler.h"
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

  LowDiscrepancySequence sequence;
  ISTATUS status = SobolSequenceAllocate(sequence.release_and_get_address());
  SuccessOrOOM(status);

  Sampler result;
  status = LowDiscrepancyImageSamplerAllocate(
      sequence.get(), pixelsamples.Get(), result.release_and_get_address());
  SuccessOrOOM(status);

  sequence.detach();

  return result;
}

}  // namespace iris