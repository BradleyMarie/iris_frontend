#ifndef _SRC_FILMS_OUTPUT_WRITER_PARSER_
#define _SRC_FILMS_OUTPUT_WRITER_PARSER_

#include "absl/strings/string_view.h"
#include "src/films/output_writers/result.h"

namespace iris {

OutputWriter ParseOutputWriter(absl::string_view file_name);

std::string GetDefaultOutputFilename();
OutputWriter CreateDefaultOutputWriter();

}  // namespace iris

#endif  // _SRC_FILMS_OUTPUT_WRITER_PARSER_