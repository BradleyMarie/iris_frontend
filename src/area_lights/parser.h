#ifndef _SRC_AREA_LIGHTS_PARSER_
#define _SRC_AREA_LIGHTS_PARSER_

#include "src/area_lights/result.h"
#include "src/common/tokenizer.h"

namespace iris {

AreaLightResult ParseAreaLight(const char* base_type_name,
                               Tokenizer& tokenizer);

}  // namespace iris

#endif  // _SRC_AREA_LIGHTS_PARSER_