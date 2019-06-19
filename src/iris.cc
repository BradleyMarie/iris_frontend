#include <cstdlib>
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>

#include "absl/flags/flag.h"
#include "absl/flags/internal/usage.h"
#include "absl/flags/parse.h"
#include "iris_advanced_toolkit/pcg_random.h"
#include "iris_physx_toolkit/sample_tracer.h"
#include "src/camera_parser.h"
#include "src/matrix_manager.h"
#include "src/ostream.h"
#include "src/scene_parser.h"
#include "src/tokenizer.h"

using iris::MatrixManager;
using iris::Random;
using iris::SampleTracer;
using iris::Tokenizer;

ABSL_FLAG(float_t, epsilon, 0.001,
          "The amount of error tolerated in distance calculations. Must be "
          "finite and greater than or equal to zero.");

ABSL_FLAG(uint32_t, num_threads, 0,
          "The number of threads to use for rendering. If zero, the "
          "number of threads will equal the number of processors in the "
          "system.");

namespace {

void ParseAndRender(const std::string& search_dir, Tokenizer& tokenizer) {
  if (absl::GetFlag(FLAGS_epsilon) < 0.0 ||
      !isfinite(absl::GetFlag(FLAGS_epsilon))) {
    std::cerr
        << "ERROR: epsilon must be finite and greater than or equal to zero"
        << std::endl;
    exit(EXIT_FAILURE);
  }

  Random rng;
  ISTATUS status = PermutedCongruentialRandomAllocate(
      0x853c49e6748fea9bULL, 0xda3e39cb94b95bdbULL,
      rng.release_and_get_address());

  switch (status) {
    case ISTATUS_ALLOCATION_FAILED:
      std::cerr << "ERROR: Allocation failed" << std::endl;
      exit(EXIT_FAILURE);
    default:
      assert(status == ISTATUS_SUCCESS);
  }

  MatrixManager matrix_manager;
  auto camera_params = ParseCamera(tokenizer, matrix_manager);
  auto scene_params =
      ParseScene(tokenizer, matrix_manager, std::get<5>(camera_params));
  auto light_sampler = std::get<4>(camera_params)(scene_params.second);
  tokenizer.GarbageCollect();

  SampleTracer sample_tracer;
  status = PhysxSampleTracerAllocate(
      std::get<3>(camera_params).detach(), scene_params.first.get(),
      light_sampler.get(), std::get<5>(camera_params).get(),
      sample_tracer.release_and_get_address());

  switch (status) {
    case ISTATUS_ALLOCATION_FAILED:
      std::cerr << "ERROR: Allocation failed" << std::endl;
      exit(EXIT_FAILURE);
    default:
      assert(status == ISTATUS_SUCCESS);
  }

  if (absl::GetFlag(FLAGS_num_threads) == 0) {
    absl::SetFlag(&FLAGS_num_threads, std::thread::hardware_concurrency());
  }

  status = IrisCameraRender(
      std::get<0>(camera_params).get(), std::get<1>(camera_params).get(),
      sample_tracer.get(), rng.get(), std::get<2>(camera_params).get(),
      absl::GetFlag(FLAGS_epsilon), absl::GetFlag(FLAGS_num_threads));

  switch (status) {
    case ISTATUS_SUCCESS:
      break;
    case ISTATUS_ALLOCATION_FAILED:
      std::cerr << "ERROR: Allocation failed" << std::endl;
      exit(EXIT_FAILURE);
    default:
      std::cerr << "ERROR: " << status << " returned from rendering"
                << std::endl;
      exit(EXIT_FAILURE);
  }

  std::get<6>(camera_params)(std::get<2>(camera_params));
}

std::string GetWorkingDirectory() { return ""; }

std::string GetParentDirectory(const std::string& file_name) {
  return file_name;
}

}  // namespace

int main(int argc, char** argv) {
  absl::flags_internal::SetProgramUsageMessage(
      "A pbrt frontend for the iris renderer.");

  auto unparsed = absl::ParseCommandLine(argc, argv);
  if (2 < unparsed.size()) {
    std::cerr << "ERROR: Only one file input supported";
    return EXIT_FAILURE;
  }

  std::string search_dir, scene;
  if (unparsed.size() == 1) {
    search_dir = GetWorkingDirectory();
    scene.assign((std::istreambuf_iterator<char>(std::cin)),
                 (std::istreambuf_iterator<char>()));
  } else {
    search_dir = GetParentDirectory(unparsed[1]);
    std::ifstream file(unparsed[1]);
    if (!file) {
      std::cerr << "ERROR: Error opening file " << unparsed[1] << std::endl;
      return EXIT_FAILURE;
    }

    scene.assign((std::istreambuf_iterator<char>(file)),
                 (std::istreambuf_iterator<char>()));
  }

  Tokenizer tokenizer(std::move(scene));
  while (tokenizer.Peek()) {
    ParseAndRender(search_dir, tokenizer);
  }

  return EXIT_SUCCESS;
}