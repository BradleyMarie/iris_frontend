#include "src/samplers/parser.h"

#include "src/common/call_directive.h"
#include "src/samplers/halton.h"
#include "src/samplers/sobol.h"
#include "src/samplers/stratified.h"

namespace iris {

Sampler ParseSampler(const char* base_type_name, Tokenizer& tokenizer) {
  return CallDirective<Sampler, 3>(
      base_type_name, tokenizer,
      {std::make_pair("halton", ParseHalton),
       std::make_pair("sobol", ParseSobol),
       std::make_pair("stratified", ParseStratified)});
}

Sampler CreateDefaultSampler() {
  Tokenizer tokenizer;
  return ParseHalton("Unused", "Unused", tokenizer);
}

}  // namespace iris