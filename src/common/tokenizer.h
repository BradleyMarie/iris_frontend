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
  Tokenizer(const std::string& search_dir);
  Tokenizer(const Tokenizer&) = delete;
  Tokenizer& operator=(const Tokenizer&) = delete;

  static std::unique_ptr<Tokenizer> CreateFromFile(
      const std::string& search_dir, const std::string& file);
  static std::unique_ptr<Tokenizer> CreateFromStream(
      const std::string& search_dir, std::istream& stream);

  std::string AbsolutePath(const std::string& file) const;
  void Include(const std::string& file);

  absl::optional<absl::string_view> Peek();
  absl::optional<absl::string_view> Next();

 private:
  std::string m_search_dir;
  std::stack<std::pair<std::reference_wrapper<std::istream>,
                       std::unique_ptr<std::istream>>>
      m_streams;
  std::string m_next;
  std::string m_peeked;
  absl::optional<bool> m_peeked_valid;
};

}  // namespace iris

#endif  // _SRC_COMMON_TOKENIZER_