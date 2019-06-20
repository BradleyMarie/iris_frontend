#ifndef _SRC_TOKENIZER_
#define _SRC_TOKENIZER_

#include "absl/strings/string_view.h"
#include "absl/types/optional.h"

namespace iris {

class Tokenizer {
 public:
  Tokenizer(const Tokenizer&) = delete;
  Tokenizer& operator=(const Tokenizer&) = delete;
  Tokenizer(std::string serialized)
      : m_serialized(std::move(serialized)), m_position(m_serialized.begin()) {}

  absl::optional<absl::string_view> Peek();
  absl::optional<absl::string_view> Next();

 private:
  absl::optional<absl::string_view> ParseNext();

  const char* ToPointer(std::string::const_iterator iter) const {
    return m_serialized.data() + (iter - m_serialized.begin());
  }

  absl::optional<absl::optional<absl::string_view>> m_next;
  std::string m_serialized;
  std::string::const_iterator m_position;
  std::string m_escaped;
};

}  // namespace iris

#endif  // _SRC_TOKENIZER_