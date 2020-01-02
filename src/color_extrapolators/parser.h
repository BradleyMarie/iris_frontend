#ifndef _SRC_COLOR_EXTRAPOLATORS_PARSER_
#define _SRC_COLOR_EXTRAPOLATORS_PARSER_

#include "src/common/pointer_types.h"
#include "src/common/tokenizer.h"

namespace iris {

ColorExtrapolator ParseColorExtrapolator(const char* base_type_name,
                                         Tokenizer& tokenizer);

ColorExtrapolator CreateDefaultColorExtrapolator();

}  // namespace iris

#endif  // _SRC_COLOR_EXTRAPOLATORS_PARSER_