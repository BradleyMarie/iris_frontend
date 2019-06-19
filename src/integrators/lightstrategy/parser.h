#ifndef _SRC_INTEGRATORS_LIGHTSTRATEGY_PARSER_
#define _SRC_INTEGRATORS_LIGHTSTRATEGY_PARSER_

#include "absl/strings/string_view.h"
#include "src/integrators/lightstrategy/result.h"

namespace iris {

LightSamplerFactory ParseLightStrategy(absl::string_view strategy);

}  // namespace iris

#endif  // _SRC_INTEGRATORS_LIGHTSTRATEGY_PARSER_