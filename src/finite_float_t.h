#ifndef _SRC_FINITE_FLOAT_T_
#define _SRC_FINITE_FLOAT_T_

#include "absl/strings/string_view.h"
#include "absl/types/optional.h"
#include "iris/iris.h"

namespace iris {

class FiniteFloatT {
 public:
  FiniteFloatT(float_t v);
  FiniteFloatT() : m_value(absl::nullopt) {}
  float_t Get() { return m_value.value(); }

 private:
  absl::optional<float_t> m_value;
};

bool ParseFiniteFloatT(absl::string_view token, FiniteFloatT* value);

}  // namespace iris

#endif  // _SRC_FINITE_FLOAT_T_