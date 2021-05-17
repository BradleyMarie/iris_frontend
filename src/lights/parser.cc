#include "src/lights/parser.h"

#include "src/lights/distant.h"
#include "src/lights/infinite.h"
#include "src/lights/point.h"

namespace iris {
namespace {

const Directive::Implementations<LightResult, SpectrumManager&, const Matrix&>
    kImpls = {{"point", ParsePoint},
              {"infinite", ParseInfinite},
              {"distant", ParseDistant}};

}  // namespace

LightResult ParseLight(Directive& directive, SpectrumManager& spectrum_manager,
                       const Matrix& model_to_world) {
  return directive.Invoke(kImpls, spectrum_manager, model_to_world);
}

}  // namespace iris