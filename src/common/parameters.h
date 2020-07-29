#ifndef _SRC_COMMON_PARAMETERS_
#define _SRC_COMMON_PARAMETERS_

#include <vector>

#include "absl/types/optional.h"
#include "absl/types/span.h"
#include "src/common/tokenizer.h"
#include "src/param_matchers/matcher.h"

namespace iris {

class Parameters {
 public:
  Parameters(absl::string_view base_type_name, absl::string_view type_name,
             Tokenizer& tokenizer);
  Parameters(Parameters&& other);
  Parameters& operator=(Parameters&& other);
  Parameters(const Parameters&) = delete;
  Parameters& operator=(const Parameters&) = delete;
  ~Parameters();

  Parameters MatchAllowUnused(absl::Span<ParamMatcher* const> param_matchers);
  void Match(absl::Span<ParamMatcher* const> param_matchers);
  void Ignore();

 private:
  Parameters(absl::string_view base_type_name, absl::string_view type_name,
             std::vector<Parameter> unused_parameters);

  absl::string_view m_base_type_name;
  absl::string_view m_type_name;
  absl::optional<std::vector<Parameter>> m_unused_parameters;
  Tokenizer* m_tokenizer;
};

}  // namespace iris

#endif  // _SRC_COMMON_PARAMETERS_