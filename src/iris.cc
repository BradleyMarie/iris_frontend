#include <iostream>
#include <sstream>
#include <thread>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/flags/usage.h"
#include "absl/flags/usage_config.h"
#include "src/directives/parser.h"
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

ABSL_FLAG(bool, welcome_message, true,
          "If true, the welcome message will not be shown.");

template <typename T>
struct Wrapper {
  absl::optional<T> opt;
};

ABSL_FLAG(Wrapper<bool>, always_compute_reflective_color, Wrapper<bool>(),
          "If true, override the eplicates an erratum in pbrt-v3 which "
          "incorrectly scales the color computed for emissive spectral power "
          "distributions by the integral of the CIE Y function. If false, the "
          "workaround is disabled and the output of iris will not match that "
          "of pbrt.");

ABSL_FLAG(Wrapper<COLOR_SPACE>, rgb_color_space, Wrapper<COLOR_SPACE>(),
          "Overrides the color space used to read RGB and color values from the"
          "input scene.");

ABSL_FLAG(Wrapper<iris::SpectralRepresentation>, spectral_representation,
          Wrapper<iris::SpectralRepresentation>(),
          "Overrides the representation used for spectra during rendering.");

bool AbslParseFlag(absl::string_view text, Wrapper<bool>* flag,
                   std::string* err) {
  if (text == "default") {
    flag->opt = absl::nullopt;
    return true;
  }

  bool value;
  bool success = absl::ParseFlag(text, &value, err);
  if (success) {
    flag->opt = value;
  }

  return success;
}

std::string AbslUnparseFlag(const Wrapper<bool>& flag) {
  if (!flag.opt.has_value()) {
    return "default";
  }
  return absl::UnparseFlag(flag.opt.value());
}

bool AbslParseFlag(absl::string_view text, Wrapper<COLOR_SPACE>* flag,
                   std::string* err) {
  if (text == "default") {
    flag->opt = absl::nullopt;
    return true;
  }

  if (text == "linear_srgb") {
    flag->opt = COLOR_SPACE_LINEAR_SRGB;
    return true;
  }

  if (text == "xyz") {
    flag->opt = COLOR_SPACE_XYZ;
    return true;
  }

  return false;
}

std::string AbslUnparseFlag(const Wrapper<COLOR_SPACE>& flag) {
  if (!flag.opt.has_value()) {
    return "default";
  }

  if (flag.opt.value() == COLOR_SPACE_LINEAR_SRGB) {
    return "linear_srgb";
  }

  if (flag.opt.value() == COLOR_SPACE_XYZ) {
    return "xyz";
  }

  assert(false);
  return "unknown";
}

bool AbslParseFlag(absl::string_view text,
                   Wrapper<iris::SpectralRepresentation>* flag,
                   std::string* err) {
  if (text == "default") {
    flag->opt = absl::nullopt;
    return true;
  }

  iris::SpectralRepresentation parsed;
  bool success = ParseSpectralRepresentation(text, &parsed);
  if (success) {
    flag->opt = parsed;
  }

  return success;
}

std::string AbslUnparseFlag(const Wrapper<iris::SpectralRepresentation>& flag) {
  if (!flag.opt.has_value()) {
    return "default";
  }

  return SpectralRepresentationToString(*flag.opt);
}

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

  iris::Parser parser;
  if (unparsed.size() == 1) {
    parser = iris::Parser::Create(std::cin);
  } else {
    parser = iris::Parser::Create(unparsed[1]);
  }

  if (absl::GetFlag(FLAGS_welcome_message)) {
    std::cout << VersionString();
  }

  for (size_t render_index = 0; !parser.Done(); render_index += 1) {
    iris::RenderToOutput(
        parser, render_index, absl::GetFlag(FLAGS_epsilon),
        absl::GetFlag(FLAGS_num_threads), absl::GetFlag(FLAGS_report_progress),
        absl::GetFlag(FLAGS_spectral_representation).opt,
        absl::GetFlag(FLAGS_rgb_color_space).opt,
        absl::GetFlag(FLAGS_always_compute_reflective_color).opt);
  }

  return EXIT_SUCCESS;
}