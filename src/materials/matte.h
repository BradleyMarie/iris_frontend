#ifndef _SRC_MATERIALS_MATTE_
#define _SRC_MATERIALS_MATTE_

#include "src/common/spectrum_manager.h"
#include "src/common/texture_manager.h"
#include "src/common/tokenizer.h"
#include "src/materials/result.h"

namespace iris {

MaterialResult ParseMatte(const char* base_type_name, const char* type_name,
                          Tokenizer& tokenizer,
                          SpectrumManager& spectrum_manager,
                          const TextureManager& texture_manager);

}  // namespace iris

#endif  // _SRC_MATERIALS_MATTE_