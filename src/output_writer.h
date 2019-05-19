#ifndef _SRC_OUTPUT_WRITER_
#define _SRC_OUTPUT_WRITER_

#include <functional>
#include <string>

#include "absl/strings/string_view.h"
#include "src/pointer_types.h"

namespace iris {

typedef std::function<void(const Framebuffer& framebuffer)> OutputWriter;

OutputWriter ParseOutputWriter(absl::string_view path);

}  // namespace iris

#endif  // _SRC_OUTPUT_WRITER_