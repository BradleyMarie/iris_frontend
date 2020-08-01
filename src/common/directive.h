#ifndef _SRC_COMMON_DIRECTIVE_
#define _SRC_COMMON_DIRECTIVE_

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
  Result Invoke(absl::Span<const std::pair<const char*,
                                           Result (*)(Parameters&, Args&...)>>
                    implementations,
                Args&... args) {
    std::vector<absl::string_view> type_names;
    for (const auto& entry : implementations) {
      type_names.push_back(entry.first);
    }
    size_t type_index = Match(type_names);
    Parameters params(m_base_type_name, type_names[type_index], *m_tokenizer);
    return implementations[type_index].second(params, args...);
  }

 private:
  size_t Match(absl::Span<const absl::string_view> type_names);

  absl::string_view m_base_type_name;
  Tokenizer* m_tokenizer;
};

}  // namespace iris

#endif  // _SRC_COMMON_DIRECTIVE_