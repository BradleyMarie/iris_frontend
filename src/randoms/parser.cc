#include "src/randoms/parser.h"

#include "src/common/call_directive.h"
#include "src/randoms/pcg.h"

namespace iris {

Random ParseRandom(absl::string_view base_type_name, Tokenizer& tokenizer) {
  return CallDirective<Random>(base_type_name, tokenizer, {{"pcg", ParsePcg}});
}

Random CreateDefaultRandom() {
  Parameters parameters;
  return ParsePcg(parameters);
}

}  // namespace iris