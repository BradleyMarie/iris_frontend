#include "src/integrators/parser.h"

#include "src/common/call_directive.h"
#include "src/integrators/path.h"

namespace iris {

IntegratorResult ParseIntegrator(const char* base_type_name,
                                 Tokenizer& tokenizer) {
  return CallDirective<IntegratorResult, 1>(
      base_type_name, tokenizer, {std::make_pair("path", ParsePath)});
}

IntegratorResult CreateDefaultIntegrator() {
  Tokenizer tokenizer;
  return ParsePath("Unused", "Unused", tokenizer);
}

}  // namespace iris