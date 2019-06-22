#ifndef _SRC_FILMS_IMAGE_
#define _SRC_FILMS_IMAGE_

#include "src/common/tokenizer.h"
#include "src/films/result.h"

namespace iris {

FilmResult ParseImage(const char* base_type_name, const char* type_name,
                      Tokenizer& tokenizer);

}  // namespace iris

#endif  // _SRC_FILMS_IMAGE_