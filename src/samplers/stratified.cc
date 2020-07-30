#include "src/samplers/stratified.h"

#include <iostream>

#include "iris_camera_toolkit/grid_image_sampler.h"
#include "src/common/error.h"
#include "src/param_matchers/integral_single.h"
#include "src/param_matchers/matcher.h"
#include "src/param_matchers/single.h"

namespace iris {
namespace {

static const bool kStratifiedSamplerDefaultJitter = false;
static const uint16_t kStratifiedSamplerDefaultXSamples = 2;
static const uint16_t kStratifiedSamplerDefaultYSamples = 2;

}  // namespace

Sampler ParseStratified(const char* base_type_name, const char* type_name,
                        Tokenizer& tokenizer) {
  SingleBoolMatcher jitter("jitter", false, kStratifiedSamplerDefaultJitter);
  NonZeroSingleUInt16Matcher xsamples("xsamples", false,
                                      kStratifiedSamplerDefaultXSamples);
  NonZeroSingleUInt16Matcher ysamples("ysamples", false,
                                      kStratifiedSamplerDefaultXSamples);
  MatchParameters(base_type_name, type_name, tokenizer,
                  {&jitter, &xsamples, &ysamples});

  Sampler result;
  ISTATUS status =
      GridImageSamplerAllocate(xsamples.Get(), ysamples.Get(), jitter.Get(), 1,
                               1, false, result.release_and_get_address());
  SuccessOrOOM(status);

  return result;
}

}  // namespace iris