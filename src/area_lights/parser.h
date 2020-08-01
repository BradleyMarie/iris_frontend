#ifndef _SRC_AREA_LIGHTS_PARSER_
#define _SRC_AREA_LIGHTS_PARSER_

#include "src/area_lights/result.h"
#include "src/common/directive.h"
#include "src/common/spectrum_manager.h"
#include "src/common/tokenizer.h"

namespace iris {

AreaLightResult ParseAreaLight(Directive& directive,
                               SpectrumManager& spectrum_manager);

}  // namespace iris

#endif  // _SRC_AREA_LIGHTS_PARSER_