#include "src/samplers/stratified.h"

#include <iostream>

#include "iris_camera_toolkit/grid_pixel_sampler.h"
#include "src/param_matchers/integral_single.h"
#include "src/param_matchers/matcher.h"
#include "src/param_matchers/single.h"

namespace iris {
namespace {

static const bool kStratifiedSamplerDefaultJitter = false;
static const uint16_t kStratifiedSamplerDefaultXSamples = 2;
static const uint16_t kStratifiedSamplerDefaultYSamples = 2;

}  // namespace

PixelSampler ParseStratified(const char* base_type_name, const char* type_name,
                             Tokenizer& tokenizer) {
  SingleBoolMatcher jitter(base_type_name, type_name, "jitter", false,
                           kStratifiedSamplerDefaultJitter);
  NonZeroSingleUInt16Matcher xsamples(base_type_name, type_name, "xsamples",
                                      false, kStratifiedSamplerDefaultXSamples);
  NonZeroSingleUInt16Matcher ysamples(base_type_name, type_name, "ysamples",
                                      false, kStratifiedSamplerDefaultXSamples);
  MatchParameters<3>(base_type_name, type_name, tokenizer,
                     {&jitter, &xsamples, &ysamples});

  PixelSampler result;
  ISTATUS status =
      GridPixelSamplerAllocate(xsamples.Get(), ysamples.Get(), jitter.Get(), 1,
                               1, false, result.release_and_get_address());
  switch (status) {
    case ISTATUS_ALLOCATION_FAILED:
      std::cerr << "ERROR: Allocation failed" << std::endl;
      exit(EXIT_FAILURE);
    default:
      assert(status == ISTATUS_SUCCESS);
  }

  return result;
}

}  // namespace iris