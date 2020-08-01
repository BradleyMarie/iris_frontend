#include "src/integrators/parser.h"

#include "src/integrators/path.h"

namespace iris {
namespace {

const Directive::Implementations<IntegratorResult> kImpls = {
    {"path", ParsePath}};

}  // namespace

IntegratorResult ParseIntegrator(Directive& directive) {
  return directive.Invoke(kImpls);
}

IntegratorResult CreateDefaultIntegrator() {
  Parameters parameters;
  return ParsePath(parameters);
}

}  // namespace iris