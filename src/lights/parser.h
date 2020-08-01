#ifndef _SRC_LIGHTS_PARSER_
#define _SRC_LIGHTS_PARSER_

#include "src/common/directive.h"
#include "src/common/spectrum_manager.h"

namespace iris {

Light ParseLight(Directive& directive, SpectrumManager& spectrum_manager,
                 const Matrix& model_to_world);

}  // namespace iris

#endif  // _SRC_LIGHTS_PARSER_