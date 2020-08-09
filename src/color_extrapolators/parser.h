#ifndef _SRC_COLOR_EXTRAPOLATORS_PARSER_
#define _SRC_COLOR_EXTRAPOLATORS_PARSER_

#include "src/common/pointer_types.h"
#include "src/common/directive.h"

namespace iris {

ColorExtrapolator ParseColorExtrapolator(Directive& directive);
ColorExtrapolator CreateDefaultColorExtrapolator();

}  // namespace iris

#endif  // _SRC_COLOR_EXTRAPOLATORS_PARSER_