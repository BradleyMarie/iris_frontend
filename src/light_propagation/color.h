#ifndef _SRC_LIGHT_PROPAGATION_COLOR_
#define _SRC_LIGHT_PROPAGATION_COLOR_

#include "src/common/tokenizer.h"
#include "src/light_propagation/result.h"

namespace iris {

LightPropagationResult ParseColor(const char* base_type_name,
                                  const char* type_name, Tokenizer& tokenizer);

}  // namespace iris

#endif  // _SRC_LIGHT_PROPAGATION_COLOR_