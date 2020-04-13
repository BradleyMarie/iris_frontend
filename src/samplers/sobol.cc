#include "src/samplers/sobol.h"

#include <iostream>

#include "iris_camera_toolkit/sobol_image_sampler.h"
#include "src/common/error.h"
#include "src/param_matchers/integral_single.h"
#include "src/param_matchers/matcher.h"

namespace iris {
namespace {

static const uint16_t kSobolSamplerDefaultPixelSamples = 16;

}  // namespace

Sampler ParseSobol(const char* base_type_name, const char* type_name,
                   Tokenizer& tokenizer) {
  NonZeroSingleUInt16Matcher pixelsamples(base_type_name, type_name,
                                          "pixelsamples", false,
                                          kSobolSamplerDefaultPixelSamples);
  MatchParameters<1>(base_type_name, type_name, tokenizer, {&pixelsamples});

  Sampler result;
  ISTATUS status = SobolImageSamplerAllocate(pixelsamples.Get(),
                                             result.release_and_get_address());
  SuccessOrOOM(status);

  return result;
}

}  // namespace iris