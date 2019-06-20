#include "src/camera_parser.h"

#include <iostream>

#include "src/cameras/parser.h"
#include "src/color_integrators/parser.h"
#include "src/films/parser.h"
#include "src/integrators/parser.h"
#include "src/matrix_parser.h"
#include "src/samplers/parser.h"

namespace iris {
namespace {

CameraConfig CreateCameraConfig(
    const Matrix& camera_to_world, absl::optional<PixelSampler>&& pixel_sampler,
    absl::optional<FilmResult>&& film_result,
    absl::optional<IntegratorResult>&& integrator_result,
    absl::optional<CameraFactory>&& camera_factory,
    absl::optional<ColorIntegrator>&& color_integrator) {
  if (!pixel_sampler) {
    pixel_sampler = CreateDefaultSampler();
  }

  if (!film_result) {
    film_result = CreateDefaultFilm();
  }

  if (!integrator_result) {
    integrator_result = CreateDefaultIntegrator();
  }

  if (!camera_factory) {
    camera_factory = CreateDefaultCamera();
  }

  if (!color_integrator) {
    color_integrator = CreateDefaultColorIntegrator();
  }

  auto camera = (*camera_factory)(film_result->first);

  return {std::move(camera),
          std::move(*pixel_sampler),
          std::move(film_result->first),
          std::move(integrator_result->first),
          std::move(integrator_result->second),
          std::move(*color_integrator),
          std::move(film_result->second)};
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

  absl::optional<PixelSampler> pixel_sampler;
  absl::optional<FilmResult> film_result;
  absl::optional<IntegratorResult> integrator_result;
  absl::optional<CameraFactory> camera_factory;
  absl::optional<ColorIntegrator> color_integrator;
  for (auto token = tokenizer.Next(); token; token = tokenizer.Next()) {
    if (token == "WorldBegin") {
      return CreateCameraConfig(
          matrix_manager.GetCurrent().first, std::move(pixel_sampler),
          std::move(film_result), std::move(integrator_result),
          std::move(camera_factory), std::move(color_integrator));
    }

    if (TryParseMatrix(*token, tokenizer, matrix_manager)) {
      continue;
    }

    if (token == "Camera") {
      ErrorIfPresent("Camera", camera_factory.has_value());
      camera_factory = ParseCamera("Camera", tokenizer, matrix_manager);
      continue;
    }

    if (token == "ColorIntegrator") {
      ErrorIfPresent("ColorIntegrator", color_integrator.has_value());
      color_integrator = ParseColorIntegrator("ColorIntegrator", tokenizer);
      continue;
    }

    if (token == "Sampler") {
      ErrorIfPresent("Sampler", pixel_sampler.has_value());
      pixel_sampler = ParseSampler("Sampler", tokenizer);
      continue;
    }

    if (token == "Film") {
      ErrorIfPresent("Sampler", film_result.has_value());
      film_result = ParseFilm("Film", tokenizer);
      continue;
    }

    if (token == "Filter") {
      // TODO
      continue;
    }

    if (token == "Integrator") {
      ErrorIfPresent("Integrator", integrator_result.has_value());
      integrator_result = ParseIntegrator("Integrator", tokenizer);
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