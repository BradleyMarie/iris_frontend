#include "src/area_lights/parser.h"

#include "src/area_lights/diffuse.h"

namespace iris {
namespace {

const Directive::Implementations<AreaLightResult, SpectrumManager&> kImpls = {
  {"diffuse", ParseDiffuse}
};

}  // namespace

AreaLightResult ParseAreaLight(Directive& directive,
                               SpectrumManager& spectrum_manager) {
  return directive.Invoke(kImpls, spectrum_manager);
}

}  // namespace iris