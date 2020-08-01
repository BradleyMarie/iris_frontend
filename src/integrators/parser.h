#ifndef _SRC_INTEGRATORS_PARSER_
#define _SRC_INTEGRATORS_PARSER_

#include "src/common/directive.h"
#include "src/integrators/result.h"

namespace iris {

IntegratorResult ParseIntegrator(Directive& directive);
IntegratorResult CreateDefaultIntegrator();

}  // namespace iris

#endif  // _SRC_INTEGRATORS_PARSER_