#ifndef _SRC_LIGHT_PROPAGATION_SPECTRUM_
#define _SRC_LIGHT_PROPAGATION_SPECTRUM_

#include "src/common/tokenizer.h"
#include "src/light_propagation/result.h"

namespace iris {

LightPropagationResult ParseSpectrum(const char* base_type_name,
                                     const char* type_name,
                                     Tokenizer& tokenizer);

}  // namespace iris

#endif  // _SRC_LIGHT_PROPAGATION_SPECTRUM_