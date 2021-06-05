#ifndef _SRC_LIGHTS_PARSER_
#define _SRC_LIGHTS_PARSER_

#include "src/common/directive.h"
#include "src/common/spectrum_manager.h"
#include "src/lights/result.h"

namespace iris {

LightResult ParseLight(Directive& directive, SpectrumManager& spectrum_manager,
                       const Matrix& model_to_world,
                       const ColorIntegrator& color_integrator);

}  // namespace iris

#endif  // _SRC_LIGHTS_PARSER_