#ifndef _SRC_RANDOMS_CIE_
#define _SRC_RANDOMS_CIE_

#include "src/pointer_types.h"
#include "src/tokenizer.h"

namespace iris {

Random ParsePcg(const char* base_type_name, const char* type_name,
                Tokenizer& tokenizer);

}  // namespace iris

#endif  // _SRC_RANDOMS_CIE_