#ifndef _SRC_CAMERAS_PERSPECTIVE_
#define _SRC_CAMERAS_PERSPECTIVE_

#include "src/cameras/result.h"
#include "src/matrix_manager.h"
#include "src/tokenizer.h"

namespace iris {

CameraFactory ParsePerspective(const char* base_type_name,
                               const char* type_name, Tokenizer& tokenizer,
                               MatrixManager& matrix_manager);

}  // namespace iris

#endif  // _SRC_CAMERAS_PERSPECTIVE_