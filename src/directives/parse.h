#ifndef _SRC_DIRECTIVES_PARSE_
#define _SRC_DIRECTIVES_PARSE_

#include <vector>

#include "src/common/pointer_types.h"
#include "src/directives/parser.h"
#include "src/films/output_writers/result.h"

namespace iris {

typedef std::tuple<Scene, LightSampler, Camera, Matrix, Sampler,
                   Integrator, ColorIntegrator, Random, Framebuffer,
                   OutputWriter>
    RenderConfiguration;

RenderConfiguration ParseDirectives(Parser& parser, bool spectral,
                                    bool spectrum_color_workaround);

}  // namespace iris

#endif  // _SRC_DIRECTIVES_PARSE_