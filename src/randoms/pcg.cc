#include "src/randoms/pcg.h"

#include "iris_advanced_toolkit/pcg_random.h"
#include "src/common/error.h"

namespace iris {
namespace {

static const uint64_t kPcgRandomDefaultState = 0x853c49e6748fea9bULL;
static const uint64_t kPcgRandomDefaultOutputSequence = 0xda3e39cb94b95bdbULL;

}  // namespace

Random ParsePcg(Parameters& parameters) {
  parameters.Match();

  Random result;
  ISTATUS status = PermutedCongruentialRandomAllocate(
      kPcgRandomDefaultState, kPcgRandomDefaultOutputSequence,
      result.release_and_get_address());
  SuccessOrOOM(status);

  return result;
}

}  // namespace iris