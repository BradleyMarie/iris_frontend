#include "src/integrators/path.h"

#include "iris_physx_toolkit/path_tracer.h"
#include "src/integrators/lightstrategy/parser.h"
#include "src/param_matchers/float_single.h"
#include "src/param_matchers/integral_single.h"
#include "src/param_matchers/matcher.h"
#include "src/param_matchers/single.h"

#include <iostream>

namespace iris {
namespace {

static const uint8_t kPathTracerDefaultMinDepth = 3;
static const uint8_t kPathTracerDefaultMaxDepth = 5;
static const float_t kPathTracerDefaultRRThreshold = (float_t)1.0;

}  // namespace

IntegratorResult ParsePath(const char* base_type_name, const char* type_name,
                           Tokenizer& tokenizer) {
  SingleStringMatcher lightsamplestrategy(
      base_type_name, type_name, "lightsamplestrategy", false,
      "uniform");  // TODO: Set default to spatial
  NonZeroSingleUInt8Matcher maxdepth(base_type_name, type_name, "maxdepth",
                                     false, kPathTracerDefaultMaxDepth);
  SingleFloatMatcher rrthreshold(base_type_name, type_name, "rrthreshold",
                                 false, true, (float_t)0.0, (float_t)1.0,
                                 kPathTracerDefaultRRThreshold);
  MatchParameters<3>(base_type_name, type_name, tokenizer,
                     {&lightsamplestrategy, &maxdepth, &rrthreshold});

  Integrator integrator;
  ISTATUS status = PathTracerAllocate(
      std::min(kPathTracerDefaultMinDepth, maxdepth.Get()), maxdepth.Get(),
      rrthreshold.Get(), integrator.release_and_get_address());
  switch (status) {
    case ISTATUS_ALLOCATION_FAILED:
      std::cerr << "ERROR: Allocation failed" << std::endl;
      exit(EXIT_FAILURE);
    default:
      assert(status == ISTATUS_SUCCESS);
  }

  return std::make_pair(std::move(integrator),
                        ParseLightStrategy(lightsamplestrategy.Get()));
}

}  // namespace iris