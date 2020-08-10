#ifndef _SRC_DIRECTIVES_PBRT_WORKAROUND_
#define _SRC_DIRECTIVES_PBRT_WORKAROUND_

#include "src/common/directive.h"

namespace iris {

bool ParseAlwaysComputeReflectiveColor(Directive& directive);
bool CreateDefaultAlwaysComputeReflectiveColor();

}  // namespace iris

#endif  // _SRC_DIRECTIVES_PBRT_WORKAROUND_