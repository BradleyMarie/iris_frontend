#ifndef _SRC_SAMPLERS_SOBOL_
#define _SRC_SAMPLERS_SOBOL_

#include "src/common/pointer_types.h"
#include "src/common/parameters.h"

namespace iris {

Sampler ParseSobol(Parameters& parameters);

}  // namespace iris

#endif  // _SRC_SAMPLERS_SOBOL_