#include "src/randoms/pcg.h"

#include "iris_advanced_toolkit/pcg_random.h"
#include "src/param_matcher.h"

#include <iostream>

namespace iris {
namespace {

static const uint64_t kPcgRandomDefaultState = 0x853c49e6748fea9bULL;
static const uint64_t kPcgRandomDefaultOutputSequence = 0xda3e39cb94b95bdbULL;

}  // namespace

Random ParsePcg(const char* base_type_name, const char* type_name,
                Tokenizer& tokenizer) {
  ParseAllParameter<0>(base_type_name, type_name, tokenizer, {});

  Random result;
  ISTATUS status = PermutedCongruentialRandomAllocate(
      kPcgRandomDefaultState, kPcgRandomDefaultOutputSequence,
      result.release_and_get_address());
  switch (status) {
    case ISTATUS_ALLOCATION_FAILED:
      std::cerr << "ERROR: Allocation failed" << std::endl;
      exit(EXIT_FAILURE);
    default:
      assert(status == ISTATUS_SUCCESS);
  }

  return result;
}

}  // namespace iris