#include "src/common/tokenizer.h"

#include <fstream>
#include <vector>

// TODO: Make this platform independent
#include <libgen.h>
#include <unistd.h>

#include "absl/strings/str_cat.h"

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

  std::cerr << "ERROR: Illegal escape character: \\" << ch << std::endl;
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

Tokenizer Tokenizer::CreateFromFile(absl::string_view file) {
  std::vector<char> file_name(file.begin(), file.end());
  file_name.push_back('\0');

  Tokenizer result;
  result.Include(file);
  result.m_search_root = dirname(file_name.data());
  return result;
}

Tokenizer Tokenizer::CreateFromStream(std::istream& stream) {
  Tokenizer result;
  result.m_streams.push(std::make_pair(std::ref(stream), nullptr));
  return result;
}

std::string Tokenizer::ResolvePath(absl::string_view file) const {
  // TODO: Make this platform independent
  if (!m_search_root.has_value() || file.empty() || file[0] == '/') {
    return std::string(file);
  }

  file.remove_prefix(1);
  return absl::StrCat(*m_search_root, "/", file);
}

void Tokenizer::Include(absl::string_view file) {
  auto buffer = absl::make_unique<std::ifstream>(ResolvePath(file));
  if (buffer->fail()) {
    std::cerr << "ERROR: Error opening file: " << file << std::endl;
    exit(EXIT_FAILURE);
  }

  m_streams.push(std::make_pair(std::ref(*buffer), std::move(buffer)));
}

absl::optional<absl::string_view> Tokenizer::Peek() {
  if (m_peeked_valid.has_value()) {
    if (*m_peeked_valid) {
      return m_peeked;
    } else {
      return absl::nullopt;
    }
  }

  for (;;) {
    if (m_streams.empty()) {
      break;
    }

    bool found = ParseNext(m_streams.top().first, m_peeked);
    if (found) {
      m_peeked_valid = found;
      return m_peeked;
    }

    m_streams.pop();
  }

  return absl::nullopt;
}

absl::optional<absl::string_view> Tokenizer::Next() {
  for (;;) {
    bool next_valid;
    if (m_peeked_valid.has_value()) {
      std::swap(m_next, m_peeked);
      next_valid = *m_peeked_valid;
      m_peeked_valid = absl::nullopt;
    } else if (!m_streams.empty()) {
      next_valid = ParseNext(m_streams.top().first, m_next);
    } else {
      break;
    }

    if (next_valid) {
      return m_next;
    } else {
      m_streams.pop();
    }
  }

  return absl::nullopt;
}

}  // namespace iris