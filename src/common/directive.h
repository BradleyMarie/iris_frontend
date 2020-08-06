#ifndef _SRC_COMMON_DIRECTIVE_
#define _SRC_COMMON_DIRECTIVE_

#include <functional>
#include <iostream>
#include <vector>

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
      std::pair<absl::string_view, std::function<Result(Parameters&, Args...)>>;

  template <typename Result, typename... Args>
  using Implementations = std::vector<Implementation<Result, Args...>>;

  template <typename... Args>
  using FormattedImplementations = std::pair<
      absl::string_view,
      absl::Span<const Implementation<void, const std::string&, Args...>>>;

  template <typename... Args>
  using AllFormattedImplementations =
      std::vector<FormattedImplementations<Args...>>;

  template <typename Result, typename... Args>
  Result Invoke(const Implementations<Result, Args...>& implementations,
                Args... args) {
    absl::InlinedVector<absl::string_view, kMaxVariantsPerDirective> type_names;
    for (const auto& entry : implementations) {
      type_names.push_back(entry.first);
    }
    size_t type_index = MatchType(type_names);
    Parameters params(m_base_type_name, type_names[type_index], *m_tokenizer);
    m_tokenizer = nullptr;
    return implementations[type_index].second(params, args...);
  }

  template <typename Result, typename... Args>
  Result Invoke(const Implementations<Result, Args&...>& implementations,
                Args&... args) {
    absl::InlinedVector<absl::string_view, kMaxVariantsPerDirective> type_names;
    for (const auto& entry : implementations) {
      type_names.push_back(entry.first);
    }
    size_t type_index = MatchType(type_names);
    Parameters params(m_base_type_name, type_names[type_index], *m_tokenizer);
    m_tokenizer = nullptr;
    return implementations[type_index].second(params, args...);
  }

  template <typename Result, typename... Args>
  std::pair<std::string, Result> InvokeNamedTyped(
      const Implementations<Result, Args&...>& implementations, Args&... args) {
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

  template <typename... Args>
  void InvokeNamedMultiFormat(
      const AllFormattedImplementations<Args&...>& implementations,
      Args&... args) {
    absl::InlinedVector<absl::string_view, kMaxVariantsPerDirective>
        format_names;
    for (const auto& entry : implementations) {
      format_names.push_back(entry.first);
    }
    std::string name = ParseName();
    size_t format_index = MatchFormat(format_names);
    absl::InlinedVector<absl::string_view, kMaxVariantsPerDirective> type_names;
    for (const auto& entry : implementations[format_index].second) {
      type_names.push_back(entry.first);
    }
    size_t type_index = MatchType(type_names);
    Parameters params(m_base_type_name, type_names[type_index], *m_tokenizer);
    m_tokenizer = nullptr;
    implementations[format_index].second[type_index].second(params, name,
                                                            args...);
  }

  void FiniteFloats(absl::Span<float_t> values,
                    absl::optional<absl::Span<std::string>> strings);
  std::string SingleString(absl::string_view field_name);

  void Empty();
  void Ignore();

 private:
  size_t MatchType(absl::Span<const absl::string_view> type_names);
  size_t MatchFormat(absl::Span<const absl::string_view> format_names);

  std::string ParseName();
  absl::string_view ParseFormat();
  bool MatchesFormat(absl::string_view type_name);
  std::pair<Parameters, size_t> MatchTyped(
      absl::Span<const absl::string_view> type_names);

  static constexpr size_t kMaxVariantsPerDirective = 20;
  absl::string_view m_base_type_name;
  Tokenizer* m_tokenizer;
};

}  // namespace iris

#endif  // _SRC_COMMON_DIRECTIVE_