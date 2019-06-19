#ifndef _SRC_INTEGRATORS_PARSER_
#define _SRC_INTEGRATORS_PARSER_

#include "src/integrators/result.h"
#include "src/tokenizer.h"

namespace iris {

IntegratorResult ParseIntegrator(const char* base_type_name,
                                 Tokenizer& tokenizer);

IntegratorResult CreateDefaultIntegrator();

}  // namespace iris

#endif  // _SRC_INTEGRATORS_PARSER_