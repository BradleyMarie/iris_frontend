#ifndef _SRC_INTEGRATORS_PARSER_
#define _SRC_INTEGRATORS_PARSER_

#include "src/common/tokenizer.h"
#include "src/integrators/result.h"

namespace iris {

IntegratorResult ParseIntegrator(absl::string_view base_type_name,
                                 Tokenizer& tokenizer);

IntegratorResult CreateDefaultIntegrator();

}  // namespace iris

#endif  // _SRC_INTEGRATORS_PARSER_