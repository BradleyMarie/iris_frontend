#include "src/integrators/parser.h"

#include "src/common/call_directive.h"
#include "src/integrators/path.h"

namespace iris {

IntegratorResult ParseIntegrator(absl::string_view base_type_name,
                                 Tokenizer& tokenizer) {
  return CallDirective<IntegratorResult>(
      base_type_name, tokenizer, {{"path", ParsePath}});
}

IntegratorResult CreateDefaultIntegrator() {
  Parameters parameters;
  return ParsePath(parameters);
}

}  // namespace iris