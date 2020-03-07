#ifndef _SRC_FILMS_OUTPUT_WRITER_EXR_
#define _SRC_FILMS_OUTPUT_WRITER_EXR_

#include "absl/strings/string_view.h"
#include "src/films/output_writers/result.h"

namespace iris {

OutputWriter ParseExr(absl::string_view file_name);

}  // namespace iris

#endif  // _SRC_FILMS_OUTPUT_WRITER_EXR_