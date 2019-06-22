#ifndef _SRC_AREA_LIGHTS_DIFFUSE_
#define _SRC_AREA_LIGHTS_DIFFUSE_

#include "src/area_lights/result.h"
#include "src/common/tokenizer.h"

namespace iris {

AreaLightResult ParseDiffuse(const char* base_type_name, const char* type_name,
                             Tokenizer& tokenizer);

}  // namespace iris

#endif  // _SRC_AREA_LIGHTS_DIFFUSE_