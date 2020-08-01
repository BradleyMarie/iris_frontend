#include "src/randoms/parser.h"

#include "src/randoms/pcg.h"

namespace iris {
namespace {

const Directive::Implementations<Random> kImpls = {{"pcg", ParsePcg}};

}  // namespace

Random ParseRandom(Directive& directive) {
  return directive.Invoke(kImpls);
}

Random CreateDefaultRandom() {
  Parameters parameters;
  return ParsePcg(parameters);
}

}  // namespace iris