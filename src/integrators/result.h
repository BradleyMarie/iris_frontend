#ifndef _SRC_INTEGRATORS_RESULT_
#define _SRC_INTEGRATORS_RESULT_

#include <utility>

#include "src/common/pointer_types.h"
#include "src/integrators/lightstrategy/result.h"

namespace iris {

typedef std::pair<Integrator, LightSamplerFactory> IntegratorResult;

}  // namespace iris

#endif  // _SRC_INTEGRATORS_RESULT_