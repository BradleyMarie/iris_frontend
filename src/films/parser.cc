#include "src/films/parser.h"

#include "src/common/call_directive.h"
#include "src/films/image.h"

namespace iris {

FilmResult ParseFilm(absl::string_view base_type_name, Tokenizer& tokenizer) {
  return CallDirective<FilmResult>(base_type_name, tokenizer,
                                   {{"image", ParseImage}});
}

FilmResult CreateDefaultFilm() {
  Parameters parameters;
  return ParseImage(parameters);
}

}  // namespace iris