#include "src/lights/distant.h"

#include <iostream>

#include "iris_physx_toolkit/directional_light.h"
#include "src/common/error.h"
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

Light ParseDistant(Parameters& parameters, SpectrumManager& spectrum_manager,
                   const Matrix& model_to_world) {
  SinglePoint3Matcher from("from", false, kPointLightDefaultFrom);
  SinglePoint3Matcher to("to", false, kPointLightDefaultTo);
  SpectrumMatcher spectrum = SpectrumMatcher::FromRgb(
      "L", false, spectrum_manager, kPointLightDefaultL);
  parameters.Match(from, to, spectrum);

  POINT3 world_from = PointMatrixMultiply(model_to_world.get(), from.Get());
  POINT3 world_to = PointMatrixMultiply(model_to_world.get(), to.Get());
  VECTOR3 world_direction = PointSubtract(world_from, world_to);

  if (!VectorValidate(world_direction)) {
    std::cerr << "ERROR: Incompatible values specified for "
              << parameters.Name() << " parameters: from, to" << std::endl;
    exit(EXIT_FAILURE);
  }

  Light result;
  ISTATUS status = DirectionalLightAllocate(
      world_direction, spectrum.Get().get(), result.release_and_get_address());
  SuccessOrOOM(status);

  return result;
}

}  // namespace iris