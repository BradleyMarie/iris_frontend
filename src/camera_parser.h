#ifndef _SRC_CAMERA_PARSER_
#define _SRC_CAMERA_PARSER_

#include <functional>
#include <string>
#include <tuple>
#include <vector>

#include "absl/strings/string_view.h"
#include "src/matrix_manager.h"
#include "src/output_writer.h"
#include "src/pointer_types.h"
#include "src/tokenizer.h"

namespace iris {

typedef std::function<LightSampler(std::vector<Light>&)> LightSamplerFactory;

typedef std::tuple<Camera, PixelSampler, Framebuffer, Integrator,
                   LightSamplerFactory, ColorIntegrator, OutputWriter>
    CameraConfig;

CameraConfig ParseCamera(Tokenizer& tokenizer, MatrixManager& matrix_manager);

}  // namespace iris

#endif  // _SRC_CAMERA_PARSER_