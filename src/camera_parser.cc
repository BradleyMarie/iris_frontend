#include <iostream>

#include "src/camera_parser.h"
#include "src/matrix_parser.h"

namespace iris {
namespace {

void ValidateResult(const CameraConfig& result) {
  std::cerr << "ERROR: ValidateResult not implemented" << std::endl;
  exit(EXIT_FAILURE);
}

}  // namespace

CameraConfig ParseCamera(Tokenizer& tokenizer, MatrixManager& matrix_manager) {
  matrix_manager.ActiveTransform(MatrixManager::ALL_TRANSFORMS);
  matrix_manager.Identity();

  CameraConfig result;
  for (auto token = tokenizer.Next(); token; token = tokenizer.Next()) {
    if (token == "WorldBegin") {
      ValidateResult(result);
      return result;
    }

    if (TryParseMatrix(*token, tokenizer, matrix_manager)) {
      continue;
    }

    if (token == "Camera") {
      // TODO
      continue;
    }

    if (token == "Sampler") {
      // TODO
      continue;
    }

    if (token == "Film") {
      // TODO
      continue;
    }

    if (token == "Filter") {
      // TODO
      continue;
    }

    if (token == "Integrator") {
      // TODO
      continue;
    }

    if (token == "Accelerator") {
      // Ignored
      continue;
    }

    if (token == "MakeNamedMedium") {
      std::cerr << "ERROR: Invalid directive before WorldBegin: " << *token
                << std::endl;
      exit(EXIT_FAILURE);
      continue;
    }

    if (token == "MediumInterface") {
      std::cerr << "ERROR: Invalid directive before WorldBegin: " << *token
                << std::endl;
      exit(EXIT_FAILURE);
      continue;
    }

    std::cerr << "ERROR: Invalid directive before WorldBegin: " << *token
              << std::endl;
    exit(EXIT_FAILURE);
  }

  std::cerr << "ERROR: Missing WorldBegin directive" << std::endl;
  exit(EXIT_FAILURE);
}

}  // namespace iris