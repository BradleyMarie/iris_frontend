#include "src/directives/spectral_representation_parser.h"

#include <iostream>

namespace iris {
namespace {

const SpectralRepresentation kDefaultSpectralRepresentation = {
    COLOR_SPACE_LINEAR_SRGB};

}  // namespace

SpectralRepresentation ParseSpectralRepresentation(Directive& directive) {
  std::string representation = directive.SingleQuotedString("type");

  SpectralRepresentation result;
  bool success = ParseSpectralRepresentation(representation, &result);
  if (!success) {
    std::cerr << "ERROR: Invalid SpectralRepresentation specified: "
              << representation << std::endl;
  }

  return result;
}

SpectralRepresentation CreateDefaultSpectralRepresentation() {
  return kDefaultSpectralRepresentation;
}

}  // namespace iris