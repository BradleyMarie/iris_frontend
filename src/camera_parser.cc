#include "src/camera_parser.h"

#include <iostream>

#include "iris_physx_toolkit/cie_color_integrator.h"
#include "src/cameras/parser.h"
#include "src/films/parser.h"
#include "src/integrators/parser.h"
#include "src/matrix_parser.h"
#include "src/samplers/parser.h"

namespace iris {
namespace {

void PopulateUninitialzedParameters(const Matrix& camera_to_world,
                                    CameraFactory camera_factory,
                                    CameraConfig& result) {
  if (!std::get<1>(result).get()) {
    std::get<1>(result) = CreateDefaultSampler();
  }

  if (!std::get<2>(result).get()) {
    auto default_film = CreateDefaultFilm();
    std::get<2>(result) = std::move(default_film.first);
    std::get<6>(result) = std::move(default_film.second);
  }

  if (!camera_factory) {
    camera_factory = CreateDefaultCamera();
  }

  auto default_integrator = CreateDefaultIntegrator();
  if (!std::get<3>(result).get()) {
    std::get<3>(result) = std::move(default_integrator.first);
  }

  if (!std::get<4>(result)) {
    std::get<4>(result) = default_integrator.second;
  }

  std::get<0>(result) = camera_factory(std::get<2>(result));

  ISTATUS status =
      CieColorIntegratorAllocate(std::get<5>(result).release_and_get_address());
  switch (status) {
    case ISTATUS_ALLOCATION_FAILED:
      std::cerr << "ERROR: Allocation failed" << std::endl;
      exit(EXIT_FAILURE);
    default:
      assert(status == ISTATUS_SUCCESS);
  }
}

void ErrorIfPresent(const char* base_type_name, bool already_set) {
  if (already_set) {
    std::cerr << "ERROR: Invalid " << base_type_name
              << " specified more than once before WorldBegin" << std::endl;
    exit(EXIT_FAILURE);
  }
}

}  // namespace

CameraConfig ParseCameraConfig(Tokenizer& tokenizer,
                               MatrixManager& matrix_manager) {
  matrix_manager.ActiveTransform(MatrixManager::ALL_TRANSFORMS);
  matrix_manager.Identity();

  CameraFactory camera_factory;
  CameraConfig result;
  for (auto token = tokenizer.Next(); token; token = tokenizer.Next()) {
    if (token == "WorldBegin") {
      PopulateUninitialzedParameters(matrix_manager.GetCurrent().first,
                                     camera_factory, result);
      return result;
    }

    if (TryParseMatrix(*token, tokenizer, matrix_manager)) {
      continue;
    }

    if (token == "Camera") {
      ErrorIfPresent("Camera", !!camera_factory);
      camera_factory = ParseCamera("Camera", tokenizer, matrix_manager);
      continue;
    }

    if (token == "Sampler") {
      ErrorIfPresent("Sampler", std::get<1>(result).get());
      std::get<1>(result) = ParseSampler("Sampler", tokenizer);
      continue;
    }

    if (token == "Film") {
      ErrorIfPresent("Sampler", std::get<2>(result).get());
      auto film = ParseFilm("Film", tokenizer);
      std::get<2>(result) = std::move(film.first);
      std::get<6>(result) = std::move(film.second);
      continue;
    }

    if (token == "Filter") {
      // TODO
      continue;
    }

    if (token == "Integrator") {
      ErrorIfPresent("Integrator", std::get<3>(result).get());
      auto integrator = ParseIntegrator("Integrator", tokenizer);
      std::get<3>(result) = std::move(integrator.first);
      std::get<4>(result) = std::move(integrator.second);
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