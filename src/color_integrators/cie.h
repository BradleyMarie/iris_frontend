#ifndef _SRC_COLOR_INTEGRATORS_CIE_
#define _SRC_COLOR_INTEGRATORS_CIE_

#include "src/common/pointer_types.h"
#include "src/common/tokenizer.h"

namespace iris {

ColorIntegrator ParseCie(const char* base_type_name, const char* type_name,
                         Tokenizer& tokenizer);

}  // namespace iris

#endif  // _SRC_COLOR_INTEGRATORS_CIE_