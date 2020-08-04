#include "src/common/parser.h"

namespace iris {

Parser Parser::CreateFromFile(absl::string_view file) {
  Parser result;
  result.m_tokenizer = Tokenizer::CreateFromFile(file);
  return result;
}

Parser Parser::CreateFromStream(std::istream& stream) {
  Parser result;
  result.m_tokenizer = Tokenizer::CreateFromStream(stream);
  return result;
}

Tokenizer& Parser::GetTokenizer() {
  return m_tokenizer;
}

bool Parser::Done() {
  return m_tokenizer.Peek().has_value();
}

}  // namespace iris