#include "src/lights/parser.h"

#include "src/common/call_directive.h"
#include "src/lights/distant.h"
#include "src/lights/point.h"

namespace iris {

Light ParseLight(absl::string_view base_type_name, Tokenizer& tokenizer,
                 SpectrumManager& spectrum_manager,
                 const Matrix& model_to_world) {
  return CallDirective<Light, SpectrumManager&, const Matrix&>(
      base_type_name, tokenizer,
      {{"point", ParsePoint}, {"distant", ParseDistant}}, spectrum_manager,
      model_to_world);
}

}  // namespace iris