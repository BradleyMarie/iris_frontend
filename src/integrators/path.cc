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

IntegratorResult ParsePath(Parameters& parameters) {
  SingleStringMatcher lightsamplestrategy(
      "lightsamplestrategy", false,
      "uniform");  // TODO: Set default to spatial
  NonZeroSingleUInt8Matcher maxdepth("maxdepth", false,
                                     kPathTracerDefaultMaxDepth);
  NonZeroSingleUInt8Matcher rrminbounces("rrminbounces", false,
                                         kPathTracerDefaultRRMinBounces);
  SingleFloatMatcher rrthreshold("rrthreshold", false, true, (float_t)0.0,
                                 INFINITY, kPathTracerDefaultRRThreshold);
  SingleFloatMatcher rrminprobability("rrminprobability", false, true,
                                      (float_t)0.0, (float_t)1.0,
                                      kPathTracerDefaultRRMinProbability);
  parameters.Match(lightsamplestrategy, maxdepth, rrminbounces,
                   rrminprobability, rrthreshold);

  Integrator integrator;
  ISTATUS status = PathTracerAllocate(
      rrminbounces.Get(), maxdepth.Get() - 1, *rrminprobability.Get(),
      *rrthreshold.Get(), integrator.release_and_get_address());
  SuccessOrOOM(status);

  return std::make_pair(std::move(integrator),
                        ParseLightStrategy(lightsamplestrategy.Get()));
}  // namespace iris

}  // namespace iris