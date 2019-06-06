#include <iostream>

#include "iris_camera_toolkit/grid_pixel_sampler.h"
#include "iris_physx_toolkit/cie_color_integrator.h"
#include "iris_physx_toolkit/one_light_sampler.h"
#include "iris_physx_toolkit/path_tracer.h"
#include "src/camera_parser.h"
#include "src/directive_parser.h"
#include "src/matrix_parser.h"
#include "src/param_matcher.h"

namespace iris {
namespace {

LightSampler CreateUniformLightSampler(std::vector<Light>& lights) {
  std::vector<PLIGHT> raw_lights(lights.size());
  for (auto& light : lights) {
    raw_lights.push_back(light.get());
  }

  LightSampler result;
  ISTATUS status = OneLightSamplerAllocate(raw_lights.data(), raw_lights.size(),
                                           result.release_and_get_address());
  switch (status) {
    case ISTATUS_ALLOCATION_FAILED:
      std::cerr << "ERROR: Allocation failed" << std::endl;
      exit(EXIT_FAILURE);
    default:
      assert(status == ISTATUS_SUCCESS);
  }

  return result;
}

LightSamplerFactory ParseLightSampleStrategy(absl::string_view strategy) {
  if (strategy == "uniform") {
    return CreateUniformLightSampler;
  }

  std::cerr << "ERROR: Unrecognized lightstrategy: " << strategy << std::endl;
  exit(EXIT_FAILURE);
}

static const bool kStratifiedSamplerDefaultJitter = false;
static const uint16_t kStratifiedSamplerDefaultXSamples = 2;
static const uint16_t kStratifiedSamplerDefaultYSamples = 2;

PixelSampler ParseStratifiedSampler(const char* base_type_name,
                                    const char* type_name, Tokenizer& tokenizer,
                                    MatrixManager& matrix_manager) {
  SingleBoolMatcher jitter(base_type_name, type_name, "jitter",
                           kStratifiedSamplerDefaultJitter);
  NonZeroSingleUInt16Matcher xsamples(base_type_name, type_name, "xsamples",
                                      kStratifiedSamplerDefaultXSamples);
  NonZeroSingleUInt16Matcher ysamples(base_type_name, type_name, "ysamples",
                                      kStratifiedSamplerDefaultXSamples);
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

static const uint8_t kPathTracerDefaultMinDepth = 3;
static const uint8_t kPathTracerDefaultMaxDepth = 5;
static const float_t kPathTracerDefaultRRThreshold = (float_t)1.0;

std::pair<Integrator, LightSamplerFactory> ParsePathIntegrator(
    const char* base_type_name, const char* type_name, Tokenizer& tokenizer,
    MatrixManager& matrix_manager) {
  SingleStringMatcher lightsamplestrategy(
      base_type_name, type_name, "lightsamplestrategy",
      "uniform");  // TODO: Set default to spatial
  NonZeroSingleUInt8Matcher maxdepth(base_type_name, type_name, "maxdepth",
                                     kPathTracerDefaultMaxDepth);
  PositiveScalarSingleFloatTMatcher rrthreshold(
      base_type_name, type_name, "rrthreshold", kPathTracerDefaultRRThreshold);
  ParseAllParameter<3>(base_type_name, type_name, tokenizer,
                       {&lightsamplestrategy, &maxdepth, &rrthreshold});

  Integrator integrator;
  ISTATUS status = PathTracerAllocate(
      std::min(kPathTracerDefaultMinDepth, maxdepth.Get()), maxdepth.Get(),
      rrthreshold.Get(), integrator.release_and_get_address());
  switch (status) {
    case ISTATUS_ALLOCATION_FAILED:
      std::cerr << "ERROR: Allocation failed" << std::endl;
      exit(EXIT_FAILURE);
    default:
      assert(status == ISTATUS_SUCCESS);
  }

  return std::make_pair(std::move(integrator),
                        ParseLightSampleStrategy(lightsamplestrategy.Get()));
}

static const char* kImageFilmDefaultFileName = "iris.pfm";
static const size_t kImageFilmDefaultXResolution = 640;
static const size_t kImageFilmDefaultYResolution = 480;

std::pair<Framebuffer, OutputWriter> ParseImageFilm(
    const char* base_type_name, const char* type_name, Tokenizer& tokenizer,
    MatrixManager& matrix_manager) {
  SingleStringMatcher filename(base_type_name, type_name, "yresolution",
                               kImageFilmDefaultFileName);
  NonZeroSingleSizeTMatcher xresolution(
      base_type_name, type_name, "xresolution", kImageFilmDefaultXResolution);
  NonZeroSingleSizeTMatcher yresolution(
      base_type_name, type_name, "yresolution", kImageFilmDefaultYResolution);
  ParseAllParameter<3>(base_type_name, type_name, tokenizer,
                       {&filename, &xresolution, &yresolution});

  Framebuffer framebuffer;
  ISTATUS status = FramebufferAllocate(xresolution.Get(), yresolution.Get(),
                                       framebuffer.release_and_get_address());
  switch (status) {
    case ISTATUS_ALLOCATION_FAILED:
      std::cerr << "ERROR: Allocation failed" << std::endl;
      exit(EXIT_FAILURE);
    default:
      assert(status == ISTATUS_SUCCESS);
  }

  return std::make_pair(std::move(framebuffer),
                        ParseOutputWriter(filename.Get()));
}

CameraConfig InitializeDefaultCameraConfig() {
  CameraConfig result;

  // Initialize Camera

  ISTATUS status = GridPixelSamplerAllocate(
      kStratifiedSamplerDefaultXSamples, kStratifiedSamplerDefaultYSamples,
      kStratifiedSamplerDefaultJitter, 1, 1, false,
      std::get<1>(result).release_and_get_address());
  switch (status) {
    case ISTATUS_ALLOCATION_FAILED:
      std::cerr << "ERROR: Allocation failed" << std::endl;
      exit(EXIT_FAILURE);
    default:
      assert(status == ISTATUS_SUCCESS);
  }

  status = FramebufferAllocate(kImageFilmDefaultXResolution,
                               kImageFilmDefaultYResolution,
                               std::get<2>(result).release_and_get_address());
  switch (status) {
    case ISTATUS_ALLOCATION_FAILED:
      std::cerr << "ERROR: Allocation failed" << std::endl;
      exit(EXIT_FAILURE);
    default:
      assert(status == ISTATUS_SUCCESS);
  }

  status =
      PathTracerAllocate(kPathTracerDefaultMinDepth, kPathTracerDefaultMaxDepth,
                         kPathTracerDefaultRRThreshold,
                         std::get<3>(result).release_and_get_address());
  switch (status) {
    case ISTATUS_ALLOCATION_FAILED:
      std::cerr << "ERROR: Allocation failed" << std::endl;
      exit(EXIT_FAILURE);
    default:
      assert(status == ISTATUS_SUCCESS);
  }

  // TODO: Set default to spatial
  std::get<4>(result) = CreateUniformLightSampler;

  status =
      CieColorIntegratorAllocate(std::get<5>(result).release_and_get_address());
  switch (status) {
    case ISTATUS_ALLOCATION_FAILED:
      std::cerr << "ERROR: Allocation failed" << std::endl;
      exit(EXIT_FAILURE);
    default:
      assert(status == ISTATUS_SUCCESS);
  }

  std::get<6>(result) = ParseOutputWriter(kImageFilmDefaultFileName);

  return result;
}

}  // namespace

CameraConfig ParseCamera(Tokenizer& tokenizer, MatrixManager& matrix_manager) {
  matrix_manager.ActiveTransform(MatrixManager::ALL_TRANSFORMS);
  matrix_manager.Identity();

  CameraConfig result = InitializeDefaultCameraConfig();
  for (auto token = tokenizer.Next(); token; token = tokenizer.Next()) {
    if (token == "WorldBegin") {
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
      auto sampler = ParseDirectiveOnce<PixelSampler, 1>(
          "Sampler", tokenizer, matrix_manager,
          {std::make_pair("stratified", ParseStratifiedSampler)},
          std::get<1>(result).get());
      std::get<1>(result) = std::move(sampler);
      continue;
    }

    if (token == "Film") {
      auto film = ParseDirectiveOnce<std::pair<Framebuffer, OutputWriter>, 1>(
          "Film", tokenizer, matrix_manager,
          {std::make_pair("image", ParseImageFilm)}, std::get<2>(result).get());
      std::get<2>(result) = std::move(film.first);
      std::get<6>(result) = std::move(film.second);
      continue;
    }

    if (token == "Filter") {
      // TODO
      continue;
    }

    if (token == "Integrator") {
      auto integrator =
          ParseDirectiveOnce<std::pair<Integrator, LightSamplerFactory>, 1>(
              "Integrator", tokenizer, matrix_manager,
              {std::make_pair("path", ParsePathIntegrator)},
              std::get<1>(result).get());
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