#ifndef _SRC_DIRECTIVES_GEOMETRY_
#define _SRC_DIRECTIVES_GEOMETRY_

#include <vector>

#include "src/common/matrix_manager.h"
#include "src/common/pointer_types.h"
#include "src/common/tokenizer.h"

namespace iris {

std::pair<Scene, std::vector<Light>> ParseGeometryDirectives(
    Tokenizer& tokenizer, const std::string& search_dir,
    MatrixManager& matrix_manager, const ColorExtrapolator& color_extrapolator,
    ColorIntegrator& color_integrator);

}  // namespace iris

#endif  // _SRC_SCENE_PARSER_