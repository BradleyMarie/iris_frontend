#ifndef _SRC_FILMS_OUTPUT_WRITER_RESULT_
#define _SRC_FILMS_OUTPUT_WRITER_RESULT_

#include <functional>

#include "src/common/pointer_types.h"

namespace iris {

typedef std::function<void(const Framebuffer& framebuffer)> OutputWriter;

}  // namespace iris

#endif  // _SRC_FILMS_OUTPUT_WRITER_RESULT_