#ifndef _SRC_COMMON_TOKENIZER_
#define _SRC_COMMON_TOKENIZER_

#include <iostream>
#include <memory>

#include "absl/strings/string_view.h"
#include "absl/types/optional.h"

namespace iris {

class Tokenizer {
 public:
  Tokenizer();
  Tokenizer(const Tokenizer&) = delete;
  Tokenizer& operator=(const Tokenizer&) = delete;
  Tokenizer(const std::string& file);
  Tokenizer(std::istream& stream) : m_stream(stream) {}

  absl::optional<absl::string_view> Peek();
  absl::optional<absl::string_view> Next();

 private:
  std::unique_ptr<std::istream> m_allocated_stream;
  std::istream& m_stream;
  std::string m_next;
  absl::optional<bool> m_next_valid;
  std::string m_peeked;
  absl::optional<bool> m_peeked_valid;
};

}  // namespace iris

#endif  // _SRC_COMMON_TOKENIZER_