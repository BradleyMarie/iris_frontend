#include "src/randoms/parser.h"

#include <iostream>

#include "src/directive_parser.h"
#include "src/randoms/pcg.h"

namespace iris {

Random ParseRandom(const char* base_type_name, Tokenizer& tokenizer) {
  return ParseDirective<Random, 1>(base_type_name, tokenizer,
                                   {std::make_pair("pcg", ParsePcg)});
}

Random CreateDefaultRandom() {
  Tokenizer tokenizer;
  return ParsePcg("Unused", "Unused", tokenizer);
}

}  // namespace iris