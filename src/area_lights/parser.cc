#include "src/area_lights/parser.h"

#include "src/area_lights/diffuse.h"
#include "src/common/directive.h"

namespace iris {
namespace {

const Directive::Implementations<AreaLightResult, SpectrumManager&> kImpls = {
  {"diffuse", ParseDiffuse}
};

}  // namespace

AreaLightResult ParseAreaLight(absl::string_view base_type_name,
                               Tokenizer& tokenizer,
                               SpectrumManager& spectrum_manager) {
  Directive directive(base_type_name, tokenizer);
  return directive.Invoke(kImpls, spectrum_manager);
}

}  // namespace iris