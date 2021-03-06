#include "src/samplers/stratified.h"

#include "iris_camera_toolkit/grid_image_sampler.h"
#include "src/common/error.h"
#include "src/param_matchers/integral_single.h"
#include "src/param_matchers/single.h"

namespace iris {
namespace {

static const bool kStratifiedSamplerDefaultJitter = false;
static const uint16_t kStratifiedSamplerDefaultXSamples = 2;
static const uint16_t kStratifiedSamplerDefaultYSamples = 2;

}  // namespace

Sampler ParseStratified(Parameters& parameters) {
  SingleBoolMatcher jitter("jitter", false, kStratifiedSamplerDefaultJitter);
  NonZeroSingleUInt16Matcher xsamples("xsamples", false,
                                      kStratifiedSamplerDefaultXSamples);
  NonZeroSingleUInt16Matcher ysamples("ysamples", false,
                                      kStratifiedSamplerDefaultXSamples);
  parameters.Match(jitter, xsamples, ysamples);

  Sampler result;
  ISTATUS status =
      GridImageSamplerAllocate(xsamples.Get(), ysamples.Get(), jitter.Get(), 1,
                               1, false, result.release_and_get_address());
  SuccessOrOOM(status);

  return result;
}

}  // namespace iris