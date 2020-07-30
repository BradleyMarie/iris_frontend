#include "src/area_lights/parser.h"

#include <iostream>

#include "src/area_lights/diffuse.h"
#include "src/common/call_directive.h"

namespace iris {

AreaLightResult ParseAreaLight(absl::string_view base_type_name,
                               Tokenizer& tokenizer,
                               SpectrumManager& spectrum_manager) {
  return CallDirective<AreaLightResult, SpectrumManager&>(
      base_type_name, tokenizer, {{"diffuse", ParseDiffuse}}, spectrum_manager);
}

}  // namespace iris