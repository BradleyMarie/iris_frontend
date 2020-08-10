#ifndef _SRC_DIRECTIVES_SPECTRAL_REPRESENTATION_PARSER_
#define _SRC_DIRECTIVES_SPECTRAL_REPRESENTATION_PARSER_

#include "src/common/directive.h"
#include "src/directives/spectral_representation.h"

namespace iris {

SpectralRepresentation ParseSpectralRepresentation(Directive& directive);
SpectralRepresentation CreateDefaultSpectralRepresentation();

}  // namespace iris

#endif  // _SRC_DIRECTIVES_SPECTRAL_REPRESENTATION_PARSER_