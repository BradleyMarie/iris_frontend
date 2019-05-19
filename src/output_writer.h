#ifndef _SRC_OUTPUT_WRITER_
#define _SRC_OUTPUT_WRITER_

#include <string>

#include "src/pointer_types.h"

namespace iris {

void WriteOutput(const Framebuffer& framebuffer, const std::string& path);

}  // namespace iris

#endif  // _SRC_OUTPUT_WRITER_