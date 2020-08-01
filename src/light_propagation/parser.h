#ifndef _SRC_LIGHT_PROPAGATION_PARSER_
#define _SRC_LIGHT_PROPAGATION_PARSER_

#include "src/common/directive.h"
#include "src/light_propagation/result.h"

namespace iris {

LightPropagationResult ParseLightPropagation(Directive& directive);
LightPropagationResult CreateDefaultLightPropagation(bool spectral);

}  // namespace iris

#endif  // _SRC_LIGHT_PROPAGATION_PARSER_