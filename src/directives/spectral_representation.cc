#include "src/directives/spectral_representation.h"

namespace iris {
namespace {

const char* kSpectralType = "spd";
const char* kLinearSrgbType = "linear_srgb";
const char* kXyzType = "xyz";

}  // namespace

bool ParseSpectralRepresentation(absl::string_view text,
                                 SpectralRepresentation* representation) {
  if (text == kSpectralType) {
    *representation = SpectralRepresentation();
    return true;
  }

  if (text == kLinearSrgbType) {
    representation->color_space = COLOR_SPACE_LINEAR_SRGB;
    return true;
  }

  if (text == kXyzType) {
    representation->color_space = COLOR_SPACE_XYZ;
    return true;
  }

  return false;
}

std::string SpectralRepresentationToString(
    const SpectralRepresentation& representation) {
  if (!representation.color_space.has_value()) {
    return kSpectralType;
  }

  switch (representation.color_space.value()) {
    default:
      assert(false);
    case COLOR_SPACE_LINEAR_SRGB:
      return kLinearSrgbType;
    case COLOR_SPACE_XYZ:
      return kXyzType;
  }
}

}  // namespace iris