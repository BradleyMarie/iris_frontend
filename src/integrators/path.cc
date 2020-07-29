#include "src/integrators/path.h"

#include <algorithm>
#include <iostream>

#include "iris_physx_toolkit/path_tracer.h"
#include "src/common/error.h"
#include "src/integrators/lightstrategy/parser.h"
#include "src/param_matchers/float_single.h"
#include "src/param_matchers/integral_single.h"
#include "src/param_matchers/matcher.h"
#include "src/param_matchers/single.h"

namespace iris {
namespace {

static const uint8_t kPathTracerDefaultMaxDepth = 5;
static const uint8_t kPathTracerDefaultRRMinBounces = 3;
static const float_t kPathTracerDefaultRRMinProbability = (float_t)0.05;
static const float_t kPathTracerDefaultRRThreshold = (float_t)1.0;

}  // namespace

IntegratorResult ParsePath(const char* base_type_name, const char* type_name,
                           Tokenizer& tokenizer) {
  SingleStringMatcher lightsamplestrategy(base_type_name, type_name,
                                          "lightsamplestrategy", false,
                                          "uniform");  // TODO: Set default to
  NonZeroSingleUInt8Matcher maxdepth(base_type_name, type_name, "maxdepth",
                                     false, kPathTracerDefaultMaxDepth);
  NonZeroSingleUInt8Matcher rrminbounces(base_type_name, type_name,
                                         "rrminbounces", false,
                                         kPathTracerDefaultRRMinBounces);
  SingleFloatMatcher rrthreshold(base_type_name, type_name, "rrthreshold",
                                 false, true, (float_t)0.0, INFINITY,
                                 kPathTracerDefaultRRThreshold);
  SingleFloatMatcher rrminprobability(
      base_type_name, type_name, "rrminprobability", false, true, (float_t)0.0,
      (float_t)1.0, kPathTracerDefaultRRMinProbability);
  MatchParameters(base_type_name, type_name, tokenizer,
                  {&lightsamplestrategy, &maxdepth, &rrminbounces,
                   &rrminprobability, &rrthreshold});

  Integrator integrator;
  ISTATUS status = PathTracerAllocate(
      rrminbounces.Get(), maxdepth.Get() - 1, *rrminprobability.Get(),
      *rrthreshold.Get(), integrator.release_and_get_address());
  SuccessOrOOM(status);

  return std::make_pair(std::move(integrator),
                        ParseLightStrategy(lightsamplestrategy.Get()));
}  // namespace iris

}  // namespace iris