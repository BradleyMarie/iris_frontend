#ifndef _SRC_RANDOMS_CIE_
#define _SRC_RANDOMS_CIE_

#include "src/common/pointer_types.h"
#include "src/common/parameters.h"

namespace iris {

Random ParsePcg(Parameters& parameters);

}  // namespace iris

#endif  // _SRC_RANDOMS_CIE_