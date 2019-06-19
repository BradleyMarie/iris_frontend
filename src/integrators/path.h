#ifndef _SRC_INTEGRATORS_PATH_
#define _SRC_INTEGRATORS_PATH_

#include "src/integrators/result.h"
#include "src/tokenizer.h"

namespace iris {

IntegratorResult ParsePath(const char* base_type_name, const char* type_name,
                           Tokenizer& tokenizer);

}  // namespace iris

#endif  // _SRC_INTEGRATORS_PATH_