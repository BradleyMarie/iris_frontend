#include "src/films/parser.h"

#include <iostream>

#include "src/common/call_directive.h"
#include "src/films/image.h"

namespace iris {

FilmResult ParseFilm(const char* base_type_name, Tokenizer& tokenizer) {
  return CallDirective<FilmResult, 1>(base_type_name, tokenizer,
                                       {std::make_pair("image", ParseImage)});
}

FilmResult CreateDefaultFilm() {
  Tokenizer tokenizer;
  return ParseImage("Unused", "Unused", tokenizer);
}

}  // namespace iris