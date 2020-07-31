#include "src/samplers/parser.h"

#include "src/common/call_directive.h"
#include "src/samplers/halton.h"
#include "src/samplers/sobol.h"
#include "src/samplers/stratified.h"

namespace iris {

Sampler ParseSampler(const char* base_type_name, Tokenizer& tokenizer) {
  return CallDirective<Sampler>(base_type_name, tokenizer,
                                {{"halton", ParseHalton},
                                 {"sobol", ParseSobol},
                                 {"stratified", ParseStratified}});
}

Sampler CreateDefaultSampler() {
  Parameters parameters;
  return ParseHalton(parameters);
}

}  // namespace iris