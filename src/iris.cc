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

ABSL_FLAG(bool, report_progress, true,
          "If false, no status bar or progress reporting will be displayed "
          "while rendering.");

ABSL_FLAG(bool, spectrum_color_workaround, true,
          "Replicates an erratum in pbrt-v3 which incorrectly scales the color "
          "computed for emissive spectral power distributions by the integral "
          "of the CIE Y function. If false, the workaround is disabled and "
          "output. If false, iris the workaround is disabled and the output "
          "of iris will not match that of pbrt.");

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

  for (size_t render_index = 0; tokenizer->Peek(); render_index += 1) {
    iris::RenderToOutput(*tokenizer, render_index, absl::GetFlag(FLAGS_epsilon),
                         absl::GetFlag(FLAGS_num_threads),
                         absl::GetFlag(FLAGS_report_progress),
                         absl::GetFlag(FLAGS_spectral),
                         absl::GetFlag(FLAGS_spectrum_color_workaround));
  }

  return EXIT_SUCCESS;
}