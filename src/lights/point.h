#ifndef _SRC_LIGHTS_POINT_
#define _SRC_LIGHTS_POINT_

#include "src/common/pointer_types.h"
#include "src/common/spectrum_manager.h"
#include "src/common/parameters.h"

namespace iris {

Light ParsePoint(Parameters& parameters, SpectrumManager& spectrum_manager,
                 const Matrix& model_to_world);

}  // namespace iris

#endif  // _SRC_LIGHTS_POINT_