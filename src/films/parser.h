#ifndef _SRC_FILMS_PARSER_
#define _SRC_FILMS_PARSER_

#include "src/common/directive.h"
#include "src/films/result.h"

namespace iris {

FilmResult ParseFilm(Directive& directive);
FilmResult CreateDefaultFilm();

}  // namespace iris

#endif  // _SRC_FILMS_PARSER_