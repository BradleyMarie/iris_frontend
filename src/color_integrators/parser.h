#ifndef _SRC_COLOR_INTEGRATORS_PARSER_
#define _SRC_COLOR_INTEGRATORS_PARSER_

#include "src/common/pointer_types.h"
#include "src/common/directive.h"

namespace iris {

ColorIntegrator ParseColorIntegrator(Directive& directive,
                                     bool spectrum_color_workaround);

ColorIntegrator CreateDefaultColorIntegrator(bool spectrum_color_workaround);

}  // namespace iris

#endif  // _SRC_COLOR_INTEGRATORS_PARSER_