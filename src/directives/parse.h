#ifndef _SRC_DIRECTIVES_PARSE_
#define _SRC_DIRECTIVES_PARSE_

#include <vector>

#include "src/common/pointer_types.h"
#include "src/common/tokenizer.h"
#include "src/films/output_writers/result.h"

namespace iris {

typedef std::tuple<Scene, LightSampler, Camera, Matrix, PixelSampler,
                   Integrator, ColorIntegrator, Random, Framebuffer,
                   OutputWriter>
    RenderConfiguration;

RenderConfiguration ParseDirectives(Tokenizer& tokenizer);

}  // namespace iris

#endif  // _SRC_DIRECTIVES_PARSE_