#ifndef _SRC_CAMERAS_PARSER_
#define _SRC_CAMERAS_PARSER_

#include "src/cameras/result.h"
#include "src/common/tokenizer.h"

namespace iris {

CameraFactory ParseCamera(absl::string_view base_type_name,
                          Tokenizer& tokenizer);

CameraFactory CreateDefaultCamera();

}  // namespace iris

#endif  // _SRC_CAMERAS_PARSER_