#include "src/samplers/parser.h"

#include "src/samplers/halton.h"
#include "src/samplers/sobol.h"
#include "src/samplers/stratified.h"

namespace iris {
namespace {

const Directive::Implementations<Sampler> kImpls = {
    {"halton", ParseHalton},
    {"sobol", ParseSobol},
    {"stratified", ParseStratified}};

}  // namespace

Sampler ParseSampler(Directive& directive) { return directive.Invoke(kImpls); }

Sampler CreateDefaultSampler() {
  Parameters parameters;
  return ParseHalton(parameters);
}

}  // namespace iris