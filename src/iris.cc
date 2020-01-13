#include <iostream>
#include <thread>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/flags/usage.h"
#include "src/common/tokenizer.h"
#include "src/render.h"

ABSL_FLAG(float_t, epsilon, 0.001,
          "The amount of error tolerated in distance calculations. Must be "
          "finite and greater than or equal to zero.");

ABSL_FLAG(uint32_t, num_threads, 0,
          "The number of threads to use for rendering. If zero, the "
          "number of threads will equal the number of processors in the "
          "system.");

ABSL_FLAG(bool, spectral, false,
          "Controls whether rendering should be fully spectral or approximate. "
          "If false, XYZ colors instead of SPD samples will be used in shading "
          "calculations.");

namespace {

std::string GetWorkingDirectory() { return ""; }

std::string GetParentDirectory(const std::string& file_name) {
  return file_name;
}

}  // namespace

int main(int argc, char** argv) {
  absl::SetProgramUsageMessage("A pbrt frontend for the iris renderer.");

  auto unparsed = absl::ParseCommandLine(argc, argv);
  if (2 < unparsed.size()) {
    std::cerr << "ERROR: Only one file input supported";
    return EXIT_FAILURE;
  }

  if (absl::GetFlag(FLAGS_epsilon) < 0.0 ||
      !std::isfinite(absl::GetFlag(FLAGS_epsilon))) {
    std::cerr
        << "ERROR: epsilon must be finite and greater than or equal to zero"
        << std::endl;
    exit(EXIT_FAILURE);
  }

  if (absl::GetFlag(FLAGS_num_threads) == 0) {
    absl::SetFlag(&FLAGS_num_threads, std::thread::hardware_concurrency());
  }

  std::unique_ptr<iris::Tokenizer> tokenizer;
  if (unparsed.size() == 1) {
    tokenizer =
        iris::Tokenizer::CreateFromStream(GetWorkingDirectory(), std::cin);
  } else {
    tokenizer = iris::Tokenizer::CreateFromFile(GetParentDirectory(unparsed[1]),
                                                unparsed[1]);
  }

  while (tokenizer->Peek()) {
    iris::RenderToOutput(*tokenizer, absl::GetFlag(FLAGS_epsilon),
                         absl::GetFlag(FLAGS_num_threads),
                         absl::GetFlag(FLAGS_spectral));
  }

  return EXIT_SUCCESS;
}