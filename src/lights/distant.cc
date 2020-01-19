#include "src/lights/distant.h"

#include <iostream>

#include "iris_physx_toolkit/directional_light.h"
#include "src/common/error.h"
#include "src/param_matchers/matcher.h"
#include "src/param_matchers/single.h"
#include "src/param_matchers/spectrum.h"

namespace iris {
namespace {

static const std::array<float_t, 3> kPointLightDefaultL = {
    (float_t)1.0, (float_t)1.0, (float_t)1.0};
static const POINT3 kPointLightDefaultFrom =
    PointCreate((float_t)0.0, (float_t)0.0, (float_t)0.0);
static const POINT3 kPointLightDefaultTo =
    PointCreate((float_t)0.0, (float_t)0.0, (float_t)1.0);

}  // namespace

Light ParseDistant(const char* base_type_name, const char* type_name,
                   Tokenizer& tokenizer, SpectrumManager& spectrum_manager,
                   const Matrix& model_to_world) {
  SinglePoint3Matcher from(base_type_name, type_name, "from", false,
                           kPointLightDefaultFrom);
  SinglePoint3Matcher to(base_type_name, type_name, "to", false,
                         kPointLightDefaultFrom);
  SpectrumMatcher spectrum =
      SpectrumMatcher::FromRgb(base_type_name, type_name, "L", false,
                               spectrum_manager, kPointLightDefaultL);
  MatchParameters<3>(base_type_name, type_name, tokenizer,
                     {&from, &to, &spectrum});

  POINT3 world_from = PointMatrixMultiply(model_to_world.get(), from.Get());
  POINT3 world_to = PointMatrixMultiply(model_to_world.get(), to.Get());
  VECTOR3 world_direction = PointSubtract(world_from, world_to);

  if (!VectorValidate(world_direction)) {
    std::cerr << "ERROR: Incompatible values specified for " << type_name << " "
              << base_type_name << " parameters: from, to" << std::endl;
    exit(EXIT_FAILURE);
  }

  Light result;
  ISTATUS status = DirectionalLightAllocate(
      world_direction, spectrum.Get().get(), result.release_and_get_address());
  SuccessOrOOM(status);

  return result;
}

}  // namespace iris