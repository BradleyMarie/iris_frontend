#include "src/directives/global.h"

#include <iostream>

#include "src/cameras/parser.h"
#include "src/color_integrators/parser.h"
#include "src/directives/include.h"
#include "src/directives/transform.h"
#include "src/films/parser.h"
#include "src/integrators/parser.h"
#include "src/light_propagation/parser.h"
#include "src/randoms/parser.h"
#include "src/samplers/parser.h"

namespace iris {
namespace {

GlobalConfig CreateGlobalConfig(
    Matrix camera_to_world, absl::optional<PixelSampler>&& pixel_sampler,
    absl::optional<FilmResult>&& film_result,
    absl::optional<IntegratorResult>&& integrator_result,
    absl::optional<CameraFactory>&& camera_factory,
    absl::optional<LightPropagationResult>&& light_propagation,
    absl::optional<ColorIntegrator>&& color_integrator,
    absl::optional<Random>&& random, bool spectral,
    bool spectrum_color_workaround) {
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

  if (!light_propagation) {
    light_propagation = CreateDefaultLightPropagation(spectral);
  }

  if (!color_integrator) {
    color_integrator = CreateDefaultColorIntegrator(spectrum_color_workaround);
  }

  if (!random) {
    random = CreateDefaultRandom();
  }

  auto camera = camera_factory.value()(film_result->first);
  auto light_propagation_params =
      light_propagation.value()(color_integrator.value());

  return std::make_tuple(
      std::move(camera), std::move(camera_to_world),
      std::move(pixel_sampler.value()), std::move(film_result->first),
      std::move(integrator_result->first), std::move(integrator_result->second),
      std::move(light_propagation_params.first),
      std::move(light_propagation_params.second),
      std::move(film_result->second), std::move(random.value()));
}

template <typename Result, typename... Args>
bool CallOnce(const char* base_type_name, absl::string_view token,
              absl::optional<Result>& result,
              Result (*function)(const char*, Tokenizer&, Args...),
              Tokenizer& tokenizer, Args... args) {
  if (token != base_type_name) {
    return false;
  }

  if (result) {
    std::cerr << "ERROR: Invalid " << base_type_name
              << " specified more than once before WorldBegin" << std::endl;
    exit(EXIT_FAILURE);
  }

  result = function(base_type_name, tokenizer, args...);

  return true;
}

}  // namespace

GlobalConfig ParseGlobalDirectives(Tokenizer& tokenizer,
                                   MatrixManager& matrix_manager, bool spectral,
                                   bool spectrum_color_workaround) {
  matrix_manager.ActiveTransform(MatrixManager::ALL_TRANSFORMS);
  matrix_manager.Identity();

  absl::optional<PixelSampler> pixel_sampler;
  absl::optional<FilmResult> film_result;
  absl::optional<IntegratorResult> integrator_result;
  absl::optional<CameraFactory> camera_factory;
  absl::optional<LightPropagationResult> light_propagation;
  absl::optional<ColorIntegrator> color_integrator;
  absl::optional<Random> random;
  Matrix camera_to_world;
  for (auto token = tokenizer.Next(); token; token = tokenizer.Next()) {
    if (token == "WorldBegin") {
      return CreateGlobalConfig(
          std::move(camera_to_world), std::move(pixel_sampler),
          std::move(film_result), std::move(integrator_result),
          std::move(camera_factory), std::move(light_propagation),
          std::move(color_integrator), std::move(random), spectral,
          spectrum_color_workaround);
    }

    if (TryParseTransformDirectives(*token, tokenizer, matrix_manager)) {
      continue;
    }

    if (TryParseInclude(*token, tokenizer)) {
      continue;
    }

    if (CallOnce<CameraFactory>("Camera", *token, camera_factory, ParseCamera,
                                tokenizer)) {
      camera_to_world = matrix_manager.GetCurrent().first;
      continue;
    }

    if (CallOnce<ColorIntegrator, bool>("ColorIntegrator", *token,
                                        color_integrator, ParseColorIntegrator,
                                        tokenizer, spectrum_color_workaround)) {
      continue;
    }

    if (CallOnce<PixelSampler>("Sampler", *token, pixel_sampler, ParseSampler,
                               tokenizer)) {
      continue;
    }

    if (CallOnce<FilmResult>("Film", *token, film_result, ParseFilm,
                             tokenizer)) {
      continue;
    }

    if (token == "PixelFilter") {
      // TODO
      continue;
    }

    if (CallOnce<IntegratorResult>("Integrator", *token, integrator_result,
                                   ParseIntegrator, tokenizer)) {
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

    if (CallOnce<LightPropagationResult>("LightPropagation", *token,
                                         light_propagation,
                                         ParseLightPropagation, tokenizer)) {
      continue;
    }

    if (CallOnce<Random>("Random", *token, random, ParseRandom, tokenizer)) {
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