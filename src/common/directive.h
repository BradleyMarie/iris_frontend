#ifndef _SRC_COMMON_DIRECTIVE_
#define _SRC_COMMON_DIRECTIVE_

#include "absl/container/inlined_vector.h"
#include "absl/types/span.h"
#include "src/common/parameters.h"

namespace iris {

class Directive {
 public:
  Directive(absl::string_view base_type_name, Tokenizer& tokenizer);
  Directive(Directive&& other) = delete;
  Directive& operator=(Directive&& other) = delete;
  Directive(const Directive&) = delete;
  Directive& operator=(const Directive&) = delete;
  ~Directive();

  template <typename Result, typename... Args>
  using Implementation =
      std::pair<absl::string_view, Result (*)(Parameters&, Args...)>;

  template <typename Result, typename... Args>
  using Implementations = absl::Span<const Implementation<Result, Args...>>;

  template <typename Result, typename... Args>
  Result Invoke(Implementations<Result, Args...> implementations,
                Args... args) {
    absl::InlinedVector<absl::string_view, kMaxVariantsPerDirective> type_names;
    for (const auto& entry : implementations) {
      type_names.push_back(entry.first);
    }
    size_t type_index = Match(type_names);
    Parameters params(m_base_type_name, type_names[type_index], *m_tokenizer);
    m_tokenizer = nullptr;
    return implementations[type_index].second(params, args...);
  }

  template <typename Result, typename... Args>
  Result Invoke(Implementations<Result, Args&...> implementations,
                Args&... args) {
    absl::InlinedVector<absl::string_view, kMaxVariantsPerDirective> type_names;
    for (const auto& entry : implementations) {
      type_names.push_back(entry.first);
    }
    size_t type_index = Match(type_names);
    Parameters params(m_base_type_name, type_names[type_index], *m_tokenizer);
    m_tokenizer = nullptr;
    return implementations[type_index].second(params, args...);
  }

 private:
  size_t Match(absl::Span<const absl::string_view> type_names);

  static constexpr size_t kMaxVariantsPerDirective = 20;
  absl::string_view m_base_type_name;
  Tokenizer* m_tokenizer;
};

}  // namespace iris

#endif  // _SRC_COMMON_DIRECTIVE_