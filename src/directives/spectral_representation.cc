#include "src/directives/spectral_representation.h"

#include "src/directives/color_space.h"

namespace iris {
namespace {

const char* kSpectralType = "spd";

}  // namespace

bool ParseSpectralRepresentation(absl::string_view text,
                                 SpectralRepresentation* representation) {
  if (text == kSpectralType) {
    *representation = SpectralRepresentation();
    return true;
  }

  COLOR_SPACE result;
  bool success = ParseColorSpace(text, &result);
  if (success) {
    representation->color_space = result;
  }

  return success;
}

std::string SpectralRepresentationToString(
    const SpectralRepresentation& representation) {
  if (!representation.color_space.has_value()) {
    return kSpectralType;
  }

  return ColorSpaceToString(representation.color_space.value());
}

}  // namespace iris