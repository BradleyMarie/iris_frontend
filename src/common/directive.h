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

  template <typename Result, typename... Args>
  std::pair<std::string, Result> InvokeNamedTyped(
      Implementations<Result, Args&...> implementations, Args&... args) {
    absl::InlinedVector<absl::string_view, kMaxVariantsPerDirective> type_names;
    for (const auto& entry : implementations) {
      type_names.push_back(entry.first);
    }
    std::string name = ParseName();
    auto match = MatchTyped(type_names);
    m_tokenizer = nullptr;
    return std::make_pair(
        name, implementations[match.second].second(match.first, args...));
  }

  template <typename Result, typename... Args>
  absl::optional<std::pair<std::string, Result>> TryInvokeNamedWithFormat(
    absl::string_view type_name,
    Implementations<Result, Args&...> implementations, Args&... args) {
    if (!MatchesFormat(type_name)) {
      return absl::nullopt;
    }
    absl::InlinedVector<absl::string_view, kMaxVariantsPerDirective> type_names;
    for (const auto& entry : implementations) {
      type_names.push_back(entry.first);
    }
    std::string name = ParseName();
    size_t type_index = Match(type_names);
    Parameters params(m_base_type_name, type_names[type_index], *m_tokenizer);
    m_tokenizer = nullptr;
    return std::make_pair(name, implementations[type_index].second(params, args...));
  }

  std::string SingleString(absl::string_view field_name);

  void Ignore();

  void FormatError [[noreturn]] ();

 private:
  size_t Match(absl::Span<const absl::string_view> type_names);

  std::string ParseName();
  bool MatchesFormat(absl::string_view type_name);
  std::pair<Parameters, size_t> MatchTyped(
      absl::Span<const absl::string_view> type_names);

  static constexpr size_t kMaxVariantsPerDirective = 20;
  absl::string_view m_base_type_name;
  absl::optional<absl::string_view> m_first_token;
  absl::optional<std::string> m_name;
  Tokenizer* m_tokenizer;
};

}  // namespace iris

#endif  // _SRC_COMMON_DIRECTIVE_