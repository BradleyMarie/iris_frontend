#include "src/films/output_writers/parser.h"

#include <iostream>

#include "absl/flags/flag.h"
#include "absl/strings/match.h"
#include "src/films/output_writers/pfm.h"

ABSL_FLAG(std::string, default_output, "iris.pfm",
          "The default output location if none is specified.");

namespace iris {

OutputWriter ParseOutputWriter(absl::string_view file_name) {
  if (!absl::EndsWith(file_name, ".pfm")) {
    std::cerr << "ERROR: pfm is the only supported output format" << std::endl;
    exit(EXIT_FAILURE);
  }

  return ParsePfm(file_name);
}

std::string GetDefaultOutputFilename() {
  return absl::GetFlag(FLAGS_default_output);
}

OutputWriter CreateDefaultOutputWriter() {
  return ParseOutputWriter(GetDefaultOutputFilename());
}

}  // namespace iris