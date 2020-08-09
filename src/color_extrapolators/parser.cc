#include "src/color_extrapolators/parser.h"

#include "src/color_extrapolators/smits.h"

namespace iris {
namespace {

const Directive::Implementations<ColorExtrapolator> kImpls = {
    {"smits", ParseSmits}};

}  // namespace

ColorExtrapolator ParseColorExtrapolator(Directive& directive) {
  return directive.Invoke(kImpls);
}

ColorExtrapolator CreateDefaultColorExtrapolator() {
  Parameters parameters;
  return ParseSmits(parameters);
}

}  // namespace iris