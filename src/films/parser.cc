#include "src/films/parser.h"

#include "src/directive_parser.h"
#include "src/films/image.h"

#include <iostream>

namespace iris {

FilmResult ParseFilm(const char* base_type_name, Tokenizer& tokenizer) {
  return ParseDirective<FilmResult, 1>(base_type_name, tokenizer,
                                       {std::make_pair("image", ParseImage)});
}

FilmResult CreateDefaultFilm() {
  Tokenizer tokenizer("");
  return ParseImage("Unused", "Unused", tokenizer);
}

}  // namespace iris