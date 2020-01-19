#ifndef _SRC_LIGHTS_PARSER_
#define _SRC_LIGHTS_PARSER_

#include "src/common/pointer_types.h"
#include "src/common/spectrum_manager.h"
#include "src/common/tokenizer.h"

namespace iris {

Light ParseLight(const char* base_type_name, Tokenizer& tokenizer,
                 SpectrumManager& spectrum_manager,
                 const Matrix& model_to_world);

}  // namespace iris

#endif  // _SRC_LIGHTS_PARSER_