#ifndef _SRC_PARAM_MATCHER_MATERIALS_MATCHER_
#define _SRC_PARAM_MATCHER_MATERIALS_MATCHER_

#include <algorithm>

#include "src/param_matchers/matcher.h"
#include "src/param_matchers/single.h"

namespace iris {

template <size_t NumParams>
void MatchMaterialParameters(
    const char* base_type_name, const char* type_name, Tokenizer& tokenizer,
    bool ignore_type,
    const std::array<ParamMatcher*, NumParams>& supported_parameters) {
  if (ignore_type) {
    SingleStringMatcher type(base_type_name, type_name, "type", false,
                             "ignored");
    std::array<ParamMatcher*, NumParams + 1> extended_supported_params;
    std::copy(supported_parameters.begin(), supported_parameters.end(),
              extended_supported_params.begin());
    extended_supported_params[NumParams + 1] = &type;
    MatchParameters<NumParams + 1>(base_type_name, type_name, tokenizer,
                                   extended_supported_params);
  } else {
    MatchParameters<NumParams>(base_type_name, type_name, tokenizer,
                               supported_parameters);
  }
}

}  // namespace iris

#endif  // _SRC_PARAM_MATCHER_MATERIALS_MATCHER_