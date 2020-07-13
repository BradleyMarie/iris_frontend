#ifndef _SRC_SHAPES_RESULT_
#define _SRC_SHAPES_RESULT_

#include <tuple>
#include <vector>

#include "src/common/pointer_types.h"

namespace iris {

enum class ShapeCoordinateSystem { Model, World };

typedef std::tuple<std::vector<Shape>,
                   std::vector<std::tuple<Shape, EmissiveMaterial, uint32_t>>,
                   ShapeCoordinateSystem>
    ShapeResult;

}  // namespace iris

#endif  // _SRC_SHAPES_RESULT_