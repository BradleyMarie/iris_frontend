#ifndef _SRC_CAMERAS_PARSER_
#define _SRC_CAMERAS_PARSER_

#include "src/cameras/result.h"
#include "src/common/matrix_manager.h"
#include "src/common/tokenizer.h"

namespace iris {

CameraFactory ParseCamera(const char* base_type_name, Tokenizer& tokenizer,
                          MatrixManager& matrix_manager);

CameraFactory CreateDefaultCamera();

}  // namespace iris

#endif  // _SRC_CAMERAS_PARSER_