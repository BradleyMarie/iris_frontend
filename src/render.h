#ifndef _SRC_RENDER_
#define _SRC_RENDER_

#include "src/common/pointer_types.h"
#include "src/directives/parser.h"
#include "src/films/output_writers/result.h"

namespace iris {

std::pair<Framebuffer, OutputWriter> RenderToFramebuffer(Parser& parser,
                                                         size_t render_index,
                                                         float_t epsilon,
                                                         size_t num_threads,
                                                         bool report_progress);

void RenderToOutput(Parser& parser, size_t render_index, float_t epsilon,
                    size_t num_threads, bool report_progress);

}  // namespace iris

#endif  // _SRC_RENDER_