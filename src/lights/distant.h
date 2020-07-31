#ifndef _SRC_LIGHTS_DISTANT_
#define _SRC_LIGHTS_DISTANT_

#include "src/common/pointer_types.h"
#include "src/common/spectrum_manager.h"
#include "src/common/parameters.h"

namespace iris {

Light ParseDistant(Parameters& parameters, SpectrumManager& spectrum_manager,
                   const Matrix& model_to_world);

}  // namespace iris

#endif  // _SRC_LIGHTS_DISTANT_