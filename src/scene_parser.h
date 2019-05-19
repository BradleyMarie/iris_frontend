#ifndef _SRC_SCENE_PARSER_
#define _SRC_SCENE_PARSER_

#include "absl/strings/string_view.h"
#include "absl/types/optional.h"
#include "src/matrix_manager.h"
#include "src/pointer_types.h"
#include "src/tokenizer.h"

namespace iris {

absl::optional<std::pair<Scene, LightSampler>> ParseScene(
    Tokenizer& tokenizer, MatrixManager& matrix_manager);

}  // namespace iris

#endif  // _SRC_SCENE_PARSER_