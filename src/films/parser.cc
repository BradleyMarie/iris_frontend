#include "src/films/parser.h"

#include "src/films/image.h"

namespace iris {
namespace {

const Directive::Implementations<FilmResult> kImpls = {
    {"image", ParseImage}};

}  // namespace

FilmResult ParseFilm(Directive& directive) {
  return directive.Invoke(kImpls);
}

FilmResult CreateDefaultFilm() {
  Parameters parameters;
  return ParseImage(parameters);
}

}  // namespace iris