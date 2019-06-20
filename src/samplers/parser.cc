#include "src/samplers/parser.h"

#include <iostream>

#include "src/directive_parser.h"
#include "src/samplers/stratified.h"

namespace iris {

PixelSampler ParseSampler(const char* base_type_name, Tokenizer& tokenizer) {
  return ParseDirective<PixelSampler, 1>(
      base_type_name, tokenizer,
      {std::make_pair("stratified", ParseStratified)});
}

PixelSampler CreateDefaultSampler() {
  Tokenizer tokenizer;
  return ParseStratified("Unused", "Unused", tokenizer);
}

}  // namespace iris