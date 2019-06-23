#include "src/cameras/parser.h"

#include <iostream>

#include "src/cameras/perspective.h"
#include "src/common/call_directive.h"

namespace iris {

CameraFactory ParseCamera(const char* base_type_name, Tokenizer& tokenizer,
                          MatrixManager& matrix_manager) {
  return CallDirective<CameraFactory, 1, MatrixManager&>(
      base_type_name, tokenizer,
      {std::make_pair("perspective", ParsePerspective)}, matrix_manager);
}

CameraFactory CreateDefaultCamera() {
  MatrixManager matrix_manager;
  Tokenizer tokenizer;
  return ParsePerspective("Unused", "Unused", tokenizer, matrix_manager);
}

}  // namespace iris