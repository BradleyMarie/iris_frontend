#include "src/samplers/parser.h"

#include <iostream>

#include "src/common/call_directive.h"
#include "src/samplers/stratified.h"

namespace iris {

Sampler ParseSampler(const char* base_type_name, Tokenizer& tokenizer) {
  return CallDirective<Sampler, 1>(
      base_type_name, tokenizer,
      {std::make_pair("stratified", ParseStratified)});
}

Sampler CreateDefaultSampler() {
  Tokenizer tokenizer("");
  return ParseStratified("Unused", "Unused", tokenizer);
}

}  // namespace iris