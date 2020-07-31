#ifndef _SRC_COMMON_PARAMETERS_
#define _SRC_COMMON_PARAMETERS_

#include <array>
#include <vector>

#include "absl/types/optional.h"
#include "absl/types/span.h"
#include "src/common/tokenizer.h"
#include "src/param_matchers/matcher.h"

namespace iris {

class Parameters {
 public:
  Parameters();
  Parameters(absl::string_view base_type_name, absl::string_view type_name,
             Tokenizer& tokenizer);
  Parameters(Parameters&& other);
  Parameters& operator=(Parameters&& other);
  Parameters(const Parameters&) = delete;
  Parameters& operator=(const Parameters&) = delete;
  ~Parameters();

  absl::string_view BaseType();
  absl::string_view Type();

  template <typename... Args>
  Parameters MatchAllowUnused(Args&... param_matchers) {
    std::array<ParamMatcher*, sizeof...(Args)> parameters = {
        {&param_matchers...}};
    return MatchAllowUnusedImpl(parameters);
  }

  template <typename... Args>
  void Match(Args&... param_matchers) {
    std::array<ParamMatcher*, sizeof...(Args)> parameters = {
        {&param_matchers...}};
    MatchImpl(parameters);
  }

  void Ignore();

  // TODO: Find a way to eliminate this function
  std::string ResolvePath(absl::string_view path) {
    assert(m_tokenizer);
    return m_tokenizer->ResolvePath(path);
  }

 private:
  Parameters(absl::string_view base_type_name, absl::string_view type_name,
             std::vector<Parameter> unused_parameters);

  Parameters MatchAllowUnusedImpl(
      absl::Span<ParamMatcher* const> param_matchers);
  void MatchImpl(absl::Span<ParamMatcher* const> param_matchers);

  absl::string_view m_base_type_name;
  absl::string_view m_type_name;
  absl::optional<std::vector<Parameter>> m_unused_parameters;
  Tokenizer* m_tokenizer;
};

}  // namespace iris

#endif  // _SRC_COMMON_PARAMETERS_