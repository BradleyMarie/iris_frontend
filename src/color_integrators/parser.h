#ifndef _SRC_COLOR_INTEGRATORS_PARSER_
#define _SRC_COLOR_INTEGRATORS_PARSER_

#include "src/common/pointer_types.h"
#include "src/common/tokenizer.h"

namespace iris {

ColorIntegrator ParseColorIntegrator(const char* base_type_name,
                                     Tokenizer& tokenizer);

ColorIntegrator CreateDefaultColorIntegrator();

}  // namespace iris

#endif  // _SRC_COLOR_INTEGRATORS_PARSER_