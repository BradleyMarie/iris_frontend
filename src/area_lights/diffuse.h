#ifndef _SRC_AREA_LIGHTS_MATTE_
#define _SRC_AREA_LIGHTS_MATTE_

#include "src/area_lights/result.h"
#include "src/tokenizer.h"

namespace iris {

AreaLightResult ParseDiffuse(const char* base_type_name, const char* type_name,
                             Tokenizer& tokenizer);

}  // namespace iris

#endif  // _SRC_AREA_LIGHTS_MATTE_