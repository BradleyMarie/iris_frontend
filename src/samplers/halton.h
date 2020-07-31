#ifndef _SRC_SAMPLERS_HALTON_
#define _SRC_SAMPLERS_HALTON_

#include "src/common/pointer_types.h"
#include "src/common/parameters.h"

namespace iris {

Sampler ParseHalton(Parameters& parameters);

}  // namespace iris

#endif  // _SRC_SAMPLERS_HALTON_