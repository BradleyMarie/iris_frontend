#include "src/param_matchers/float_single.h"

namespace iris {

const size_t SingleFloatMatcher::m_variant_type =
    GetIndex<FloatParameter, ParameterData>();

}  // namespace iris