#ifndef _SRC_FILMS_OUTPUT_WRITER_PFM_
#define _SRC_FILMS_OUTPUT_WRITER_PFM_

#include "absl/strings/string_view.h"
#include "src/films/output_writers/result.h"

namespace iris {

OutputWriter ParsePfm(absl::string_view file_name);

}  // namespace iris

#endif  // _SRC_FILMS_OUTPUT_WRITER_PFM_