#ifndef _SRC_MATERIALS_RESULT_
#define _SRC_MATERIALS_RESULT_

#include <set>
#include <utility>

#include "src/common/pointer_types.h"

namespace iris {

typedef std::pair<Material, std::set<Reflector>> MaterialResult;

}  // namespace iris

#endif  // _SRC_MATERIALS_RESULT_