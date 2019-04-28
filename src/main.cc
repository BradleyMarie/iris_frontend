#include <cstdlib>

#include "gflags/gflags.h"

DEFINE_uint32(num_threads, 0,
              "The number of threads to use for rendering. If zero, the "
              "number of threads will equal to the number of processors in "
              "the system.");

int main(int argc, char **argv) {
  gflags::SetUsageMessage("A pbrt frontend for the iris renderer.");
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  return EXIT_SUCCESS;
}