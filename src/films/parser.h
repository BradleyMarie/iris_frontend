#ifndef _SRC_FILMS_PARSER_
#define _SRC_FILMS_PARSER_

#include "src/common/tokenizer.h"
#include "src/films/result.h"

namespace iris {

FilmResult ParseFilm(absl::string_view base_type_name, Tokenizer& tokenizer);

FilmResult CreateDefaultFilm();

}  // namespace iris

#endif  // _SRC_FILMS_PARSER_