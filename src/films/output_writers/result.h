#ifndef _SRC_FILMS_OUTPUT_WRITER_RESULT_
#define _SRC_FILMS_OUTPUT_WRITER_RESULT_

#include <memory>

#include "src/common/pointer_types.h"

namespace iris {

class OutputWriterBase {
 public:
  virtual void Write(const Framebuffer& framebuffer) = 0;
  virtual ~OutputWriterBase() {}
};

typedef std::unique_ptr<OutputWriterBase> OutputWriter;

}  // namespace iris

#endif  // _SRC_FILMS_OUTPUT_WRITER_RESULT_