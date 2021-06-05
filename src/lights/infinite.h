#ifndef _SRC_LIGHTS_INFINITE_
#define _SRC_LIGHTS_INFINITE_

#include "src/common/parameters.h"
#include "src/common/pointer_types.h"
#include "src/common/spectrum_manager.h"
#include "src/lights/result.h"

namespace iris {

LightResult ParseInfinite(Parameters& parameters,
                          SpectrumManager& spectrum_manager,
                          const Matrix& model_to_world,
                          const ColorIntegrator& color_integrator);

}  // namespace iris

#endif  // _SRC_LIGHTS_INFINITE_