#ifndef _SRC_COLOR_INTEGRATORS_PARSER_
#define _SRC_COLOR_INTEGRATORS_PARSER_

#include "src/common/pointer_types.h"
#include "src/common/tokenizer.h"

namespace iris {

ColorIntegrator ParseColorIntegrator(absl::string_view base_type_name,
                                     Tokenizer& tokenizer,
                                     bool spectrum_color_workaround);

ColorIntegrator CreateDefaultColorIntegrator(bool spectrum_color_workaround);

}  // namespace iris

#endif  // _SRC_COLOR_INTEGRATORS_PARSER_