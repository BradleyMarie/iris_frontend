#ifndef _SRC_FILMS_RESULT_
#define _SRC_FILMS_RESULT_

#include <utility>

#include "src/common/pointer_types.h"
#include "src/films/output_writers/result.h"

namespace iris {

typedef std::pair<Framebuffer, OutputWriter> FilmResult;

}  // namespace iris

#endif  // _SRC_FILMS_RESULT_