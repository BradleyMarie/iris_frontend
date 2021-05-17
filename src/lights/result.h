#ifndef _SRC_LIGHTS_RESULT_
#define _SRC_LIGHTS_RESULT_

#include <tuple>

#include "src/common/pointer_types.h"

namespace iris {

typedef std::tuple<Light, EnvironmentalLight> LightResult;

}  // namespace iris

#endif  // _SRC_LIGHTS_RESULT_