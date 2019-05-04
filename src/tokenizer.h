#include "absl/strings/string_view.h"
#include "absl/types/optional.h"

namespace iris {

class Tokenizer {
 public:
  Tokenizer() : Tokenizer("") {}
  Tokenizer(std::string serialized)
      : m_serialized(std::move(serialized)), m_position(m_serialized.begin()) {}
  static Tokenizer FromConsole();
  static Tokenizer FromFile(const std::string& path);

  absl::optional<absl::string_view> Next();

 private:
  const char* ToPointer(std::string::const_iterator iter) const {
    return m_serialized.data() + (iter - m_serialized.begin());
  }

  std::string m_serialized;
  std::string::const_iterator m_position;
  std::string m_escaped;
};

}  // namespace iris