#ifndef _SRC_DIRECTIVES_GLOBAL_
#define _SRC_DIRECTIVES_GLOBAL_

#include <tuple>

#include "src/common/matrix_manager.h"
#include "src/common/pointer_types.h"
#include "src/common/tokenizer.h"
#include "src/films/output_writers/result.h"
#include "src/integrators/lightstrategy/result.h"

namespace iris {

typedef std::tuple<Camera, PixelSampler, Framebuffer, Integrator,
                   LightSamplerFactory, ColorExtrapolator, ColorIntegrator,
                   OutputWriter, Random>
    GlobalConfig;

GlobalConfig ParseGlobalDirectives(Tokenizer& tokenizer,
                                   const std::string& search_dir,
                                   MatrixManager& matrix_manager);

}  // namespace iris

#endif  // _SRC_DIRECTIVES_GLOBAL_