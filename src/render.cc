#include "src/render.h"

#include <iostream>

#include "iris_physx_toolkit/sample_tracer.h"
#include "src/common/error.h"
#include "src/common/ostream.h"
#include "src/directives/parse.h"

namespace iris {

std::pair<Framebuffer, OutputWriter> RenderToFramebuffer(
    Tokenizer& tokenizer, const std::string& search_dir, float_t epsilon,
    size_t num_threads) {
  assert(isfinite(epsilon) && (float_t)0.0 <= epsilon);
  assert(num_threads != 0);

  auto render_config = ParseDirectives(tokenizer);

  SampleTracer sample_tracer;
  ISTATUS status = PhysxSampleTracerAllocate(
      std::get<4>(render_config).detach(), std::get<0>(render_config).get(),
      std::get<1>(render_config).get(), std::get<5>(render_config).get(),
      sample_tracer.release_and_get_address());
  SuccessOrOOM(status);

  status = IrisCameraRender(
      std::get<2>(render_config).get(), std::get<3>(render_config).get(),
      sample_tracer.get(), std::get<6>(render_config).get(),
      std::get<7>(render_config).get(), epsilon, num_threads);

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

  return std::make_pair(std::move(std::get<7>(render_config)),
                        std::move(std::get<8>(render_config)));
}

void RenderToOutput(Tokenizer& tokenizer, const std::string& search_dir,
                    float_t epsilon, size_t num_threads) {
  auto render_result =
      RenderToFramebuffer(tokenizer, search_dir, epsilon, num_threads);
  render_result.second(render_result.first);
}

}  // namespace iris