#ifndef _SRC_LIGHTS_POINT_
#define _SRC_LIGHTS_POINT_

#include "src/common/parameters.h"
#include "src/common/pointer_types.h"
#include "src/common/spectrum_manager.h"
#include "src/lights/result.h"

namespace iris {

LightResult ParsePoint(Parameters& parameters,
                       SpectrumManager& spectrum_manager,
                       const Matrix& model_to_world);

}  // namespace iris

#endif  // _SRC_LIGHTS_POINT_