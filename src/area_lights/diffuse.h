#ifndef _SRC_AREA_LIGHTS_DIFFUSE_
#define _SRC_AREA_LIGHTS_DIFFUSE_

#include "src/area_lights/result.h"
#include "src/common/spectrum_manager.h"
#include "src/common/parameters.h"

namespace iris {

AreaLightResult ParseDiffuse(Parameters& parameters,
                             SpectrumManager& spectrum_manager);

}  // namespace iris

#endif  // _SRC_AREA_LIGHTS_DIFFUSE_