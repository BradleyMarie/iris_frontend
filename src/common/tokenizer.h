#ifndef _SRC_COMMON_TOKENIZER_
#define _SRC_COMMON_TOKENIZER_

#include <iostream>
#include <memory>
#include <stack>

#include "absl/strings/string_view.h"
#include "absl/types/optional.h"

namespace iris {

class Tokenizer {
 public:
  Tokenizer() = default;
  Tokenizer(const Tokenizer&) = delete;
  Tokenizer& operator=(const Tokenizer&) = delete;
  Tokenizer(Tokenizer&& other) = default;
  Tokenizer& operator=(Tokenizer&& other) = default;

  static Tokenizer CreateFromFile(absl::string_view file_path);
  static Tokenizer CreateFromStream(std::istream& stream);

  std::string AbsolutePath(absl::string_view file_path) const;
  void Include(absl::string_view file_path);

  absl::optional<absl::string_view> Peek();
  absl::optional<absl::string_view> Next();

 private:
  std::stack<std::pair<std::reference_wrapper<std::istream>,
                       std::unique_ptr<std::istream>>>
      m_streams;
  std::string m_next;
  std::string m_peeked;
  absl::optional<bool> m_peeked_valid;
};

}  // namespace iris

#endif  // _SRC_COMMON_TOKENIZER_