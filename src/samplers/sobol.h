#ifndef _SRC_SAMPLERS_SOBOL_
#define _SRC_SAMPLERS_SOBOL_

#include "src/common/pointer_types.h"
#include "src/common/tokenizer.h"

namespace iris {

Sampler ParseSobol(const char* base_type_name, const char* type_name,
                   Tokenizer& tokenizer);

}  // namespace iris

#endif  // _SRC_SAMPLERS_SOBOL_