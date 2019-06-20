#include "src/tokenizer.h"

#include <cctype>
#include <iostream>

namespace iris {
namespace {

static char ReadEscapedCharacter(char ch) {
  switch (ch) {
    case 'b':
      return '\b';
    case 'f':
      return '\f';
    case 'n':
      return '\n';
    case 'r':
      return '\r';
    case 't':
      return '\t';
    case '\\':
      return '\\';
    case '\'':
      return '\'';
    case '"':
      return '"';
  }

  std::cerr << "ERROR: Illegal escape character \\" << ch << std::endl;
  exit(EXIT_FAILURE);
}

}  // namespace

absl::optional<absl::string_view> Tokenizer::Peek() {
  if (!m_next) {
    m_next = ParseNext();
  }

  return *m_next;
}

absl::optional<absl::string_view> Tokenizer::Next() {
  if (m_next) {
    auto result = *m_next;
    m_next = absl::nullopt;
    return result;
  }

  return ParseNext();
}

absl::optional<absl::string_view> Tokenizer::ParseNext() {
  while (m_position != m_serialized.end()) {
    const char* token_start = ToPointer(m_position++);
    if (std::isspace(static_cast<unsigned char>(*token_start))) {
      continue;
    }

    if (*token_start == '"') {
      bool escaped = false;
      bool found = false;
      while (m_position != m_serialized.end()) {
        char current = *m_position++;

        if (current == '"') {
          found = true;
          break;
        }

        if (current == '\n') {
          std::cerr << "ERROR: New line found before end of quoted text"
                    << std::endl;
          exit(EXIT_FAILURE);
        }

        if (current == '\\') {
          escaped = true;
          m_position++;
        }
      }

      if (!found) {
        std::cerr << "ERROR: Unterminated quoted text" << std::endl;
        exit(EXIT_FAILURE);
      }

      if (!escaped) {
        return absl::string_view(token_start,
                                 ToPointer(m_position) - token_start);
      }

      m_escaped.clear();
      for (auto ptr = token_start; ptr != ToPointer(m_position); ptr++) {
        if (*ptr != '\\') {
          m_escaped += ReadEscapedCharacter(*(++ptr));
        } else {
          m_escaped += *ptr;
        }
      }

      return absl::string_view(m_escaped);
    }

    if (*token_start == '[' || *token_start == ']') {
      return absl::string_view(token_start, 1);
    }

    if (*token_start == '#') {
      while (m_position != m_serialized.end()) {
        char current = *m_position++;

        if (current == '\r' || current == '\n') {
          break;
        }
      }

      continue;
    }

    while (m_position != m_serialized.end()) {
      if (std::isspace(static_cast<unsigned char>(*m_position)) ||
          *m_position == '"' || *m_position == '[' || *m_position == ']') {
        break;
      }
      m_position++;
    }

    return absl::string_view(token_start, ToPointer(m_position) - token_start);
  }

  return absl::nullopt;
}

}  // namespace iris