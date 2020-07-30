#ifndef _SRC_COLOR_INTEGRATORS_CIE_
#define _SRC_COLOR_INTEGRATORS_CIE_

#include "src/common/parameters.h"
#include "src/common/pointer_types.h"

namespace iris {

ColorIntegrator ParseCie(Parameters& parameters,
                         bool spectrum_color_workaround);

}  // namespace iris

#endif  // _SRC_COLOR_INTEGRATORS_CIE_