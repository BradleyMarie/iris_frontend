#ifndef _SRC_SAMPLERS_PARSER_
#define _SRC_SAMPLERS_PARSER_

#include "src/common/pointer_types.h"
#include "src/common/directive.h"

namespace iris {

Sampler ParseSampler(Directive& directive);
Sampler CreateDefaultSampler();

}  // namespace iris

#endif  // _SRC_SAMPLERS_PARSER_