#include "src/cameras/parser.h"

#include <iostream>

#include "src/cameras/perspective.h"
#include "src/common/call_directive.h"

namespace iris {

CameraFactory ParseCamera(const char* base_type_name, Tokenizer& tokenizer) {
  return CallDirective<CameraFactory, 1>(
      base_type_name, tokenizer,
      {std::make_pair("perspective", ParsePerspective)});
}

CameraFactory CreateDefaultCamera() {
  Tokenizer tokenizer;
  return ParsePerspective("Unused", "Unused", tokenizer);
}

}  // namespace iris