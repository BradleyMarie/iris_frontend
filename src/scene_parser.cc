#include <iostream>

#include "src/scene_parser.h"

namespace iris {

std::pair<Scene, std::vector<Light>> ParseScene(
    Tokenizer& tokenizer, MatrixManager& matrix_manager,
    ColorIntegrator& color_integrator) {
  std::cerr << "ERROR: ParseScene not implemented" << std::endl;
  exit(EXIT_FAILURE);

  matrix_manager.ActiveTransform(MatrixManager::ALL_TRANSFORMS);
  matrix_manager.Identity();
}

}  // namespace iris