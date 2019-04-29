#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

#include "gflags/gflags.h"

DEFINE_uint32(num_threads, 0,
              "The number of threads to use for rendering. If zero, the "
              "number of threads will equal the number of processors in the "
              "system.");

namespace {

void ParseAndRenderScene(const std::string& search_dir, std::string&& scene) {}

std::string ReadToString(std::istream& stream) {
  std::string input;
  for (std::string line; std::getline(stream, line);) {
    input += line;
  }
  return input;
}

std::string GetWorkingDirectory() { return ""; }

std::string GetParentDirectory(const std::string& file_name) {
  return file_name;
}

}  // namespace

int main(int argc, char** argv) {
  gflags::SetUsageMessage("A pbrt frontend for the iris renderer.");
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  if (argc == 1) {
    std::string input = ReadToString(std::cin);
    ParseAndRenderScene(GetWorkingDirectory(), std::move(input));
  } else {
    for (int i = 1; i < argc; i++) {
      std::ifstream file(argv[i]);
      if (!file.is_open()) {
        std::cerr << "ERROR: Could not open file " << argv[i];
        return EXIT_FAILURE;
      }

      std::string input = ReadToString(file);
      file.close();

      ParseAndRenderScene(GetParentDirectory(argv[i]), std::move(input));
    }
  }

  return EXIT_SUCCESS;
}