#include <iostream>

#include "iris_camera_toolkit/grid_pixel_sampler.h"
#include "src/camera_parser.h"
#include "src/matrix_parser.h"
#include "src/param_parser.h"

namespace iris {
namespace {

void ValidateResult(const CameraConfig& result) {
  std::cerr << "ERROR: ValidateResult not implemented" << std::endl;
  exit(EXIT_FAILURE);
}

PixelSampler ParseSampler(Tokenizer& tokenizer) {
  auto token = tokenizer.Next();
  if (!token) {
    std::cerr << "ERROR: Sampler type not specified" << std::endl;
    exit(EXIT_FAILURE);
  }

  if (token != "stratified") {
    std::cerr << "ERROR: Invalid Sampler specified: " << *token << std::endl;
    exit(EXIT_FAILURE);
  }

  absl::optional<bool> jitter;
  absl::optional<uint16_t> xsamples;
  absl::optional<uint16_t> ysamples;

  for (auto param = ParseNextParam(tokenizer); param;
       param = ParseNextParam(tokenizer)) {
    if (param->first == "xsamples" &&
        absl::holds_alternative<IntParameter>(param->second) &&
        absl::get<IntParameter>(param->second).data.size() == 1) {
      if (xsamples) {
        std::cerr << "ERROR: Duplicate parameter definition: " << param->first
                  << std::endl;
        exit(EXIT_FAILURE);
      }

      auto value = absl::get<IntParameter>(param->second).data[0];
      if (value <= 0 || UINT16_MAX < value) {
        std::cerr << "ERROR: Out of range parameter: " << param->first
                  << std::endl;
        exit(EXIT_FAILURE);
      }

      xsamples = value;
      continue;
    }

    if (param->first == "ysamples" &&
        absl::holds_alternative<IntParameter>(param->second) &&
        absl::get<IntParameter>(param->second).data.size() == 1) {
      if (xsamples) {
        std::cerr << "ERROR: Duplicate parameter definition: " << param->first
                  << std::endl;
        exit(EXIT_FAILURE);
      }

      auto value = absl::get<IntParameter>(param->second).data[0];
      if (value <= 0 || UINT16_MAX < value) {
        std::cerr << "ERROR: Out of range parameter: " << param->first
                  << std::endl;
        exit(EXIT_FAILURE);
      }

      ysamples = value;
      continue;
    }

    if (param->first == "jitter" &&
        absl::holds_alternative<BoolParameter>(param->second) &&
        absl::get<BoolParameter>(param->second).data.size() == 1) {
      if (jitter) {
        std::cerr << "ERROR: Duplicate parameter definition: " << param->first
                  << std::endl;
        exit(EXIT_FAILURE);
      }

      jitter = absl::get<BoolParameter>(param->second).data[0];
      continue;
    }

    std::cerr << "ERROR: Unrecognized or misconfigured parameter to stratified "
                 "Sampler: "
              << param->first << std::endl;
    exit(EXIT_FAILURE);
  }

  PixelSampler result;
  ISTATUS status = GridPixelSamplerAllocate(
      xsamples.value_or(2), ysamples.value_or(2), jitter.value_or(true), 1, 1,
      false, result.release_and_get_address());
  switch (status) {
    case ISTATUS_ALLOCATION_FAILED:
      std::cerr << "ERROR: Allocation failed" << std::endl;
      exit(EXIT_FAILURE);
    default:
      assert(status == ISTATUS_SUCCESS);
  }

  return result;
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
      auto sampler = ParseSampler(tokenizer);
      std::get<1>(result) = std::move(sampler);
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

    if (token == "TransformTimes") {
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