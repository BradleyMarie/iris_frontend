#include "src/render.h"

#include <iostream>
#include <string>

#include "iris_camera_toolkit/status_bar_progress_reporter.h"
#include "iris_physx_toolkit/sample_tracer.h"
#include "src/common/error.h"
#include "src/common/ostream.h"
#include "src/directives/parser.h"

namespace iris {

std::pair<Framebuffer, OutputWriter> RenderToFramebuffer(
    Parser& parser, size_t render_index, float_t epsilon, size_t num_threads,
    bool report_progress,
    absl::optional<SpectralRepresentation> spectral_representation_override,
    absl::optional<COLOR_SPACE> rgb_color_space_override,
    absl::optional<bool> always_compute_reflective_color_override) {
  assert(isfinite(epsilon) && (float_t)0.0 <= epsilon);
  assert(num_threads != 0);

  auto render_config =
      *parser.Next(spectral_representation_override, rgb_color_space_override,
                   always_compute_reflective_color_override);

  ISTATUS status = IntegratorPrepare(
      std::get<5>(render_config).get(), std::get<0>(render_config).get(),
      std::get<1>(render_config).get(), std::get<6>(render_config).get());
  SuccessOrOOM(status);

  SampleTracer sample_tracer;
  status = PhysxSampleTracerAllocate(std::get<5>(render_config).detach(),
                                     sample_tracer.release_and_get_address());
  SuccessOrOOM(status);

  ProgressReporter progress_reporter;
  if (report_progress) {
    std::string progress_label;
    if (parser.Done() && render_index == 0) {
      progress_label = "Rendering";
    } else {
      progress_label = "Rendering (" + std::to_string(render_index + 1) + ")";
    }

    status = StatusBarProgressReporterAllocate(
        progress_label.c_str(), progress_reporter.release_and_get_address());
    SuccessOrOOM(status);
  }

  status = IrisCameraRender(
      std::get<2>(render_config).get(), std::get<3>(render_config).get(),
      std::get<4>(render_config).get(), sample_tracer.get(),
      std::get<7>(render_config).get(), std::get<8>(render_config).get(),
      progress_reporter.get(), epsilon, num_threads);

  switch (status) {
    case ISTATUS_SUCCESS:
      break;
    case ISTATUS_ALLOCATION_FAILED:
      ReportOOM();
    default:
      std::cerr << "ERROR: " << status << " returned from rendering"
                << std::endl;
      exit(EXIT_FAILURE);
  }

  return std::make_pair(std::move(std::get<8>(render_config)),
                        std::move(std::get<9>(render_config)));
}

void RenderToOutput(
    Parser& parser, size_t render_index, float_t epsilon, size_t num_threads,
    bool report_progress,
    absl::optional<SpectralRepresentation> spectral_representation_override,
    absl::optional<COLOR_SPACE> rgb_color_space_override,
    absl::optional<bool> always_compute_reflective_color_override) {
  auto render_result = RenderToFramebuffer(
      parser, render_index, epsilon, num_threads, report_progress,
      spectral_representation_override, rgb_color_space_override,
      always_compute_reflective_color_override);
  render_result.second->Write(render_result.first);
}

}  // namespace iris