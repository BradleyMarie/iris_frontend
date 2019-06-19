#ifndef _SRC_INTEGRATORS_RESULT_
#define _SRC_INTEGRATORS_RESULT_

#include <utility>

#include "src/integrators/lightstrategy/result.h"
#include "src/pointer_types.h"

namespace iris {

typedef std::pair<Integrator, LightSamplerFactory> IntegratorResult;

}  // namespace iris

#endif  // _SRC_INTEGRATORS_RESULT_