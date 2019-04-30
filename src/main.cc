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

  if (2 < argc) {
    std::cerr << "ERROR: Only one file input supported";
    return EXIT_FAILURE;
  }

  std::string input, search_dir;
  if (argc == 1) {
    input = ReadToString(std::cin);
    search_dir = GetWorkingDirectory();
  } else {
    std::ifstream file(argv[1]);
    if (!file.is_open()) {
      std::cerr << "ERROR: Could not open file " << argv[1];
      return EXIT_FAILURE;
    }

    input = ReadToString(file);
    search_dir = GetParentDirectory(argv[1]);
  }

  ParseAndRenderScene(search_dir, std::move(input));

  return EXIT_SUCCESS;
}