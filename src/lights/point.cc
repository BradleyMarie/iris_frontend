#include "src/lights/point.h"

#include "iris_physx_toolkit/point_light.h"
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

}  // namespace

Light ParsePoint(const char* base_type_name, const char* type_name,
                 Tokenizer& tokenizer, SpectrumManager& spectrum_manager,
                 const Matrix& model_to_world) {
  SinglePoint3Matcher from(base_type_name, type_name, "from", false,
                           kPointLightDefaultFrom);
  SpectrumMatcher spectrum =
      SpectrumMatcher::FromRgb(base_type_name, type_name, "L", false,
                               tokenizer, spectrum_manager, kPointLightDefaultL);
  MatchParameters(base_type_name, type_name, tokenizer, {&from, &spectrum});

  POINT3 world_from = PointMatrixMultiply(model_to_world.get(), from.Get());

  Light result;
  ISTATUS status = PointLightAllocate(world_from, spectrum.Get().get(),
                                      result.release_and_get_address());
  SuccessOrOOM(status);

  return result;
}

}  // namespace iris