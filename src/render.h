#ifndef _SRC_RENDER_
#define _SRC_RENDER_

#include "src/common/pointer_types.h"
#include "src/directives/parser.h"
#include "src/films/output_writers/result.h"

namespace iris {

std::pair<Framebuffer, OutputWriter> RenderToFramebuffer(
    Parser& parser, size_t render_index, float_t epsilon, size_t num_threads,
    bool report_progress,
    absl::optional<SpectralRepresentation> spectral_representation_override,
    absl::optional<COLOR_SPACE> rgb_color_space_override,
    absl::optional<bool> always_compute_reflective_color_override);

void RenderToOutput(
    Parser& parser, size_t render_index, float_t epsilon, size_t num_threads,
    bool report_progress,
    absl::optional<SpectralRepresentation> spectral_representation_override,
    absl::optional<COLOR_SPACE> rgb_color_space_override,
    absl::optional<bool> always_compute_reflective_color_override);

}  // namespace iris

#endif  // _SRC_RENDER_