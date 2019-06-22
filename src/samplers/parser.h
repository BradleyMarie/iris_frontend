#ifndef _SRC_SAMPLERS_PARSER_
#define _SRC_SAMPLERS_PARSER_

#include "src/common/pointer_types.h"
#include "src/common/tokenizer.h"

namespace iris {

PixelSampler ParseSampler(const char* base_type_name, Tokenizer& tokenizer);

PixelSampler CreateDefaultSampler();

}  // namespace iris

#endif  // _SRC_SAMPLERS_PARSER_