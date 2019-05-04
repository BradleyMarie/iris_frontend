#include <cstdlib>
#include <iostream>
#include <vector>

#include "gflags/gflags.h"
#include "src/tokenizer.h"

using iris::Tokenizer;

DEFINE_uint32(num_threads, 0,
              "The number of threads to use for rendering. If zero, the "
              "number of threads will equal the number of processors in the "
              "system.");

namespace {

void ParseAndRenderScene(const std::string& search_dir, Tokenizer tokenizer) {
  for (auto token = tokenizer.Next(); token; token = tokenizer.Next()) {
    std::cout << "token: '" << *token << "'" << std::endl;
  }
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

  Tokenizer tokenizer;
  std::string search_dir;
  if (argc == 1) {
    tokenizer = Tokenizer::FromConsole();
    search_dir = GetWorkingDirectory();
  } else {
    tokenizer = Tokenizer::FromFile(argv[1]);
    search_dir = GetParentDirectory(argv[1]);
  }

  ParseAndRenderScene(search_dir, std::move(tokenizer));

  return EXIT_SUCCESS;
}