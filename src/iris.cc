#include <iostream>
#include <thread>
#include <vector>

#include "absl/flags/flag.h"
#include "absl/flags/internal/usage.h"
#include "absl/flags/parse.h"
#include "iris_physx_toolkit/sample_tracer.h"
#include "src/common/ostream.h"
#include "src/common/tokenizer.h"
#include "src/directives/parse.h"

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

  auto render_config = ParseDirectives(tokenizer);

  SampleTracer sample_tracer;
  ISTATUS status = PhysxSampleTracerAllocate(
      std::get<4>(render_config).detach(), std::get<0>(render_config).get(),
      std::get<1>(render_config).get(), std::get<5>(render_config).get(),
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
      std::get<2>(render_config).get(), std::get<3>(render_config).get(),
      sample_tracer.get(), std::get<6>(render_config).get(),
      std::get<7>(render_config).get(), absl::GetFlag(FLAGS_epsilon),
      absl::GetFlag(FLAGS_num_threads));

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

  std::get<8>(render_config)(std::get<7>(render_config));
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

  std::unique_ptr<Tokenizer> tokenizer;
  std::string search_dir;
  if (unparsed.size() == 1) {
    search_dir = GetWorkingDirectory();
    tokenizer = absl::make_unique<Tokenizer>(std::cin);
  } else {
    search_dir = GetParentDirectory(unparsed[1]);
    tokenizer = absl::make_unique<Tokenizer>(unparsed[1]);
  }

  while (tokenizer->Peek()) {
    ParseAndRender(search_dir, *tokenizer);
  }

  return EXIT_SUCCESS;
}