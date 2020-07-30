#include "src/cameras/parser.h"

#include "src/cameras/perspective.h"
#include "src/common/call_directive.h"

namespace iris {

CameraFactory ParseCamera(const char* base_type_name, Tokenizer& tokenizer) {
  return CallDirective<CameraFactory>(base_type_name, tokenizer,
                                      {{"perspective", ParsePerspective}});
}

CameraFactory CreateDefaultCamera() {
  Parameters parameters;
  return ParsePerspective(parameters);
}

}  // namespace iris