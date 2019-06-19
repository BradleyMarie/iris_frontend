#ifndef _SRC_SAMPLERS_STRATIFIED_
#define _SRC_SAMPLERS_STRATIFIED_

#include "src/pointer_types.h"
#include "src/tokenizer.h"

namespace iris {

PixelSampler ParseStratified(const char* base_type_name, const char* type_name,
                             Tokenizer& tokenizer);

}  // namespace iris

#endif  // _SRC_SAMPLERS_STRATIFIED_