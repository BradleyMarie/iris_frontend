#ifndef _SRC_COLOR_EXTRAPOLATORS_CIE_
#define _SRC_COLOR_EXTRAPOLATORS_CIE_

#include "src/common/pointer_types.h"
#include "src/common/tokenizer.h"

namespace iris {

ColorExtrapolator ParseSmits(const char* base_type_name, const char* type_name,
                             Tokenizer& tokenizer);

}  // namespace iris

#endif  // _SRC_COLOR_EXTRAPOLATORS_CIE_