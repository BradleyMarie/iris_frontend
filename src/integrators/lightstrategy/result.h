#ifndef _SRC_INTEGRATORS_LIGHTSTRATEGY_RESULT_
#define _SRC_INTEGRATORS_LIGHTSTRATEGY_RESULT_

#include <functional>
#include <vector>

#include "src/pointer_types.h"

namespace iris {

typedef std::function<LightSampler(std::vector<Light>&)> LightSamplerFactory;

}  // namespace iris

#endif  // _SRC_INTEGRATORS_LIGHTSTRATEGY_RESULT_