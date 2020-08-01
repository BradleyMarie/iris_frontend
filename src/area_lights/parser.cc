#include "src/area_lights/parser.h"

#include "src/area_lights/diffuse.h"
#include "src/common/directive.h"

namespace iris {

AreaLightResult ParseAreaLight(absl::string_view base_type_name,
                               Tokenizer& tokenizer,
                               SpectrumManager& spectrum_manager) {
  Directive directive(base_type_name, tokenizer);
  return directive.Invoke(
      absl::MakeConstSpan({std::make_pair("diffuse", ParseDiffuse)}),
      spectrum_manager);
}

}  // namespace iris