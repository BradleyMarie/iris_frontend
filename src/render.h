#ifndef _SRC_RENDER_
#define _SRC_RENDER_

#include "src/common/pointer_types.h"
#include "src/common/tokenizer.h"
#include "src/films/output_writers/result.h"

namespace iris {

std::pair<Framebuffer, OutputWriter> RenderToFramebuffer(Tokenizer& tokenizer,
                                                         float_t epsilon,
                                                         size_t num_threads);

void RenderToOutput(Tokenizer& tokenizer, float_t epsilon, size_t num_threads);

}  // namespace iris

#endif  // _SRC_RENDER_