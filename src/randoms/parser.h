#ifndef _SRC_RANDOMS_PARSER_
#define _SRC_RANDOMS_PARSER_

#include "src/common/pointer_types.h"
#include "src/common/directive.h"

namespace iris {

Random ParseRandom(Directive& directive);
Random CreateDefaultRandom();

}  // namespace iris

#endif  // _SRC_RANDOMS_PARSER_