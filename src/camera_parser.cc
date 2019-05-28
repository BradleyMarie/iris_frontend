#include <iostream>

#include "iris_camera_toolkit/grid_pixel_sampler.h"
#include "src/camera_parser.h"
#include "src/directive_parser.h"
#include "src/matrix_parser.h"
#include "src/param_matcher.h"

namespace iris {
namespace {

void ValidateResult(const CameraConfig& result) {
  std::cerr << "ERROR: ValidateResult not implemented" << std::endl;
  exit(EXIT_FAILURE);
}

PixelSampler ParseStratifiedSampler(const char* base_type_name,
                                    const char* type_name, Tokenizer& tokenizer,
                                    MatrixManager& matrix_manager) {
  SingleBoolMatcher jitter(base_type_name, type_name, "jitter", false);
  NonZeroSingleUInt16Matcher xsamples(base_type_name, type_name, "xsamples", 2);
  NonZeroSingleUInt16Matcher ysamples(base_type_name, type_name, "ysamples", 2);
  ParseAllParameter<3>(base_type_name, type_name, tokenizer,
                       {&jitter, &xsamples, &ysamples});

  PixelSampler result;
  ISTATUS status =
      GridPixelSamplerAllocate(xsamples.Get(), ysamples.Get(), jitter.Get(), 1,
                               1, false, result.release_and_get_address());
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
      auto sampler = ParseDirective<PixelSampler, 1>(
          "Sampler", tokenizer, matrix_manager,
          {std::make_pair("stratified", ParseStratifiedSampler)});
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