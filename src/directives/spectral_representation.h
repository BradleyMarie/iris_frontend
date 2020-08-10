#ifndef _SRC_DIRECTIVES_SPECTRAL_REPRESENTATION_
#define _SRC_DIRECTIVES_SPECTRAL_REPRESENTATION_

#include "absl/strings/string_view.h"
#include "absl/types/optional.h"
#include "iris_advanced/iris_advanced.h"

namespace iris {

struct SpectralRepresentation {
  absl::optional<COLOR_SPACE> color_space;
};

bool ParseSpectralRepresentation(absl::string_view text,
                                 SpectralRepresentation* representation);

std::string SpectralRepresentationToString(
    const SpectralRepresentation& representation);

}  // namespace iris

#endif  // _SRC_DIRECTIVES_SPECTRAL_REPRESENTATION_