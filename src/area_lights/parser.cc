#include "src/area_lights/parser.h"

#include <iostream>

#include "src/area_lights/diffuse.h"
#include "src/common/call_directive.h"

namespace iris {

AreaLightResult ParseAreaLight(const char* base_type_name, Tokenizer& tokenizer,
                               SpectrumManager& spectrum_manager) {
  return CallDirective<AreaLightResult, 1, SpectrumManager&>(
      base_type_name, tokenizer, {std::make_pair("diffuse", ParseDiffuse)},
      spectrum_manager);
}

}  // namespace iris