#ifndef _SRC_LIGHT_PROPAGATION_RESULT_
#define _SRC_LIGHT_PROPAGATION_RESULT_

#include <functional>

#include "src/common/pointer_types.h"
#include "src/common/spectrum_manager.h"

namespace iris {

typedef std::function<std::pair<SpectrumManager, ColorIntegrator>(
    const ColorIntegrator& color_integrator)>
    LightPropagationResult;

}  // namespace iris

#endif  // _SRC_LIGHT_PROPAGATION_RESULT_