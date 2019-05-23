#include "src/finite_float_t.h"

#include "absl/strings/numbers.h"

namespace iris {

FiniteFloatT::FiniteFloatT(float_t v) {
  if (isfinite(v)) {
    m_value = v;
  }
}

bool ParseFiniteFloatT(absl::string_view token, FiniteFloatT* value) {
  double as_double;
  bool success = absl::SimpleAtod(token, &as_double);
  if (!success) {
    return false;
  }

  float_t as_float = static_cast<float_t>(as_double);
  if (!isfinite(as_float)) {
    return false;
  }

  *value = FiniteFloatT(as_float);
  return true;
}

FiniteNonZeroFloatT::FiniteNonZeroFloatT(float_t v) {
  if (isfinite(v) && v != (float_t)0.0) {
    m_value = v;
  }
}

bool ParseFiniteNonZeroFloatT(absl::string_view token,
                              FiniteNonZeroFloatT* value) {
  double as_double;
  bool success = absl::SimpleAtod(token, &as_double);
  if (!success) {
    return false;
  }

  float_t as_float = static_cast<float_t>(as_double);
  if (!isfinite(as_float) || as_float == (float_t)0.0) {
    return false;
  }

  *value = FiniteNonZeroFloatT(as_float);
  return true;
}

}  // namespace iris