#ifndef _SRC_SCENE_PARSER_
#define _SRC_SCENE_PARSER_

#include <vector>

#include "src/common/matrix_manager.h"
#include "src/common/pointer_types.h"
#include "src/common/tokenizer.h"

namespace iris {

std::pair<Scene, std::vector<Light>> ParseScene(
    Tokenizer& tokenizer, MatrixManager& matrix_manager,
    ColorIntegrator& color_integrator);

}  // namespace iris

#endif  // _SRC_SCENE_PARSER_