#ifndef _SRC_CAMERA_PARSER_
#define _SRC_CAMERA_PARSER_

#include <tuple>

#include "src/common/matrix_manager.h"
#include "src/common/pointer_types.h"
#include "src/common/tokenizer.h"
#include "src/films/output_writers/result.h"
#include "src/integrators/lightstrategy/result.h"

namespace iris {

typedef std::tuple<Camera, PixelSampler, Framebuffer, Integrator,
                   LightSamplerFactory, ColorIntegrator, OutputWriter, Random>
    CameraConfig;

CameraConfig ParseCameraConfig(Tokenizer& tokenizer,
                               MatrixManager& matrix_manager);

}  // namespace iris

#endif  // _SRC_CAMERA_PARSER_