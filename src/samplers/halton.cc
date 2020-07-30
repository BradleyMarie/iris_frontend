#include "src/samplers/halton.h"

#include <iostream>

#include "iris_camera_toolkit/halton_image_sampler.h"
#include "src/common/error.h"
#include "src/param_matchers/integral_single.h"
#include "src/param_matchers/matcher.h"

namespace iris {
namespace {

static const uint16_t kHaltonSamplerDefaultPixelSamples = 16;

}  // namespace

Sampler ParseHalton(const char* base_type_name, const char* type_name,
                    Tokenizer& tokenizer) {
  NonZeroSingleUInt16Matcher pixelsamples("pixelsamples", false,
                                          kHaltonSamplerDefaultPixelSamples);
  MatchParameters(base_type_name, type_name, tokenizer, {&pixelsamples});

  Sampler result;
  ISTATUS status = HaltonImageSamplerAllocate(pixelsamples.Get(),
                                              result.release_and_get_address());
  SuccessOrOOM(status);

  return result;
}

}  // namespace iris