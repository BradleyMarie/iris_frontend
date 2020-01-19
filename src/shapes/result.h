#ifndef _SRC_SHAPES_RESULT_
#define _SRC_SHAPES_RESULT_

#include <tuple>
#include <vector>

#include "src/common/pointer_types.h"

namespace iris {

typedef std::pair<std::vector<Shape>,
                  std::vector<std::tuple<Shape, EmissiveMaterial, uint32_t>>>
    ShapeResult;

}  // namespace iris

#endif  // _SRC_SHAPES_RESULT_