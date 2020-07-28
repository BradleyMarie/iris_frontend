#include <iostream>
#include <sstream>
#include <thread>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/flags/usage.h"
#include "absl/flags/usage_config.h"
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

ABSL_FLAG(bool, welcome_message, true,
          "If true, the welcome message will not be shown.");

namespace {

#if defined NDEBUG
static const std::string kDebug;
#else
static const std::string kDebug = " debug";
#endif  // defined NDEBUG

static const std::string kVersion = "0.1";
static const std::string kBits = (sizeof(void*) == 4) ? "32-bit" : "64-bit";
static_assert(sizeof(void*) == 4 || sizeof(void*) == 8);

std::string VersionString() {
  std::stringstream result;
  result << "iris " << kVersion << " (" << kBits << kDebug << ") ["
         << std::thread::hardware_concurrency() << " cores detected]"
         << std::endl;
  result << "Copyright (C) 2020 Brad Weinberger" << std::endl;
  result << "This is free software licensed under version 3 of the GNU General "
            "Public License"
         << std::endl;
  result << "This program comes with ABSOLUTELY NO WARRANTY" << std::endl;
  return result.str();
}

static const absl::FlagsUsageConfig flags_usage_config = {
    nullptr, nullptr, nullptr, VersionString, nullptr};

}  // namespace

int main(int argc, char** argv) {
  absl::SetProgramUsageMessage(
      "A pbrt frontend for the iris renderer\n\nUsage: iris [options] file");
  absl::SetFlagsUsageConfig(flags_usage_config);

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

  iris::Tokenizer tokenizer;
  if (unparsed.size() == 1) {
    tokenizer = iris::Tokenizer::CreateFromStream(std::cin);
  } else {
    tokenizer = iris::Tokenizer::CreateFromFile(unparsed[1]);
  }

  if (absl::GetFlag(FLAGS_welcome_message)) {
    std::cout << VersionString();
  }

  for (size_t render_index = 0; tokenizer.Peek(); render_index += 1) {
    iris::RenderToOutput(tokenizer, render_index, absl::GetFlag(FLAGS_epsilon),
                         absl::GetFlag(FLAGS_num_threads),
                         absl::GetFlag(FLAGS_report_progress),
                         absl::GetFlag(FLAGS_spectral),
                         absl::GetFlag(FLAGS_spectrum_color_workaround));
  }

  return EXIT_SUCCESS;
}