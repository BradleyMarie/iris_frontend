#ifndef _SRC_AREA_LIGHTS_RESULT_
#define _SRC_AREA_LIGHTS_RESULT_

#include <set>
#include <tuple>

#include "src/common/pointer_types.h"

namespace iris {

typedef std::tuple<EmissiveMaterial, EmissiveMaterial, std::set<Spectrum>>
    AreaLightResult;

}  // namespace iris

#endif  // _SRC_AREA_LIGHTS_RESULT_