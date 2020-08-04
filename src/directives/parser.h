#ifndef _SRC_DIRECTIVES_PARSER_
#define _SRC_DIRECTIVES_PARSER_

#include "src/common/tokenizer.h"

namespace iris {

class Parser {
 public:
  Parser() = default;
  Parser(const Parser&) = delete;
  Parser& operator=(const Parser&) = delete;
  Parser(Parser&& other) = default;
  Parser& operator=(Parser&& other) = default;

  static Parser CreateFromFile(absl::string_view file_path);
  static Parser CreateFromStream(std::istream& stream);

  Tokenizer& GetTokenizer();
  bool Done();

 private:
  Tokenizer m_tokenizer;
};

}  // namespace iris

#endif  // _SRC_DIRECTIVES_PARSER_