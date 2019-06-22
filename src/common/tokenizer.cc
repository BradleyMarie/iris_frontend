#include "src/common/tokenizer.h"

#include <cctype>
#include <fstream>
#include <sstream>

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

bool ParseNext(std::istream& stream, std::string& output) {
  output.clear();

  for (int read = stream.get(); read != EOF; read = stream.get()) {
    if (std::isspace(read)) {
      continue;
    }

    char ch = static_cast<char>(read);
    if (ch == '"') {
      output += ch;
      bool escaped = false;
      bool found = false;

      for (read = stream.get(); read != EOF; read = stream.get()) {
        ch = static_cast<char>(read);
        output += ch;

        if (ch == '"') {
          found = true;
          break;
        }

        if (ch == '\n') {
          std::cerr << "ERROR: New line found before end of quoted text"
                    << std::endl;
          exit(EXIT_FAILURE);
        }

        if (ch == '\\') {
          escaped = true;
        }
      }

      if (!found) {
        std::cerr << "ERROR: Unterminated quoted text" << std::endl;
        exit(EXIT_FAILURE);
      }

      if (escaped) {
        size_t end = 0;
        for (size_t i = 0; i < output.size(); i++) {
          if (output[i] == '\\') {
            if (++i == output.size()) {
              std::cerr
                  << "ERROR: A token may not end with the escape character"
                  << std::endl;
              exit(EXIT_FAILURE);
            }
            output[end] += ReadEscapedCharacter(output[i]);
          } else {
            output[end] += output[i];
          }

          end++;
        }

        output.resize(end);
      }

      return true;
    }

    if (ch == '[' || ch == ']') {
      output += ch;
      return true;
    }

    if (ch == '#') {
      for (read = stream.get(); read != EOF; read = stream.get()) {
        ch = static_cast<char>(read);
        if (ch == '\r' || ch == '\n') {
          break;
        }
      }

      continue;
    }

    output += ch;
    for (int peeked = stream.peek(); peeked != EOF; peeked = stream.peek()) {
      if (std::isspace(peeked) || peeked == '"' || peeked == '[' ||
          peeked == ']') {
        break;
      }

      stream >> ch;
      output += ch;
    }

    return true;
  }

  return false;
}

}  // namespace

Tokenizer::Tokenizer()
    : m_allocated_stream(new std::stringstream()),
      m_stream(*m_allocated_stream) {}

Tokenizer::Tokenizer(const std::string& file)
    : m_allocated_stream(new std::ifstream(file)),
      m_stream(*m_allocated_stream) {
  if (!m_stream) {
    std::cerr << "ERROR: Error opening file " << file << std::endl;
    exit(EXIT_FAILURE);
  }
}

absl::optional<absl::string_view> Tokenizer::Peek() {
  if (!m_peeked_valid.has_value()) {
    m_peeked_valid = ParseNext(m_stream, m_peeked);
  }

  if (*m_peeked_valid) {
    return m_peeked;
  }

  return absl::nullopt;
}

absl::optional<absl::string_view> Tokenizer::Next() {
  if (m_peeked_valid.has_value()) {
    std::swap(m_next, m_peeked);
    m_next_valid = m_peeked_valid;
    m_peeked_valid = absl::nullopt;
  } else {
    m_next_valid = ParseNext(m_stream, m_next);
  }

  if (*m_next_valid) {
    return m_next;
  }

  return absl::nullopt;
}

}  // namespace iris