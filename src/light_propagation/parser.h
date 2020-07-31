#ifndef _SRC_LIGHT_PROPAGATION_PARSER_
#define _SRC_LIGHT_PROPAGATION_PARSER_

#include "src/common/tokenizer.h"
#include "src/light_propagation/result.h"

namespace iris {

LightPropagationResult ParseLightPropagation(absl::string_view base_type_name,
                                             Tokenizer& tokenizer);

LightPropagationResult CreateDefaultLightPropagation(bool spectral);

}  // namespace iris

#endif  // _SRC_LIGHT_PROPAGATION_PARSER_