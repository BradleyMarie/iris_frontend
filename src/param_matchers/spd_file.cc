#include "src/param_matchers/spd_file.h"

#include <fstream>

#include "absl/strings/numbers.h"

namespace iris {
namespace {

absl::optional<float_t> ParseFloat(const std::vector<char>& token) {
  float_t parsed;
  bool success =
      absl::SimpleAtof(absl::string_view(token.data(), token.size()), &parsed);
  if (!success) {
    return absl::nullopt;
  }
  return parsed;
}

}  // namespace

absl::optional<std::vector<float_t>> ReadSpdFile(const std::string& file,
                                                 std::istream& stream) {
  std::vector<float_t> result;
  std::vector<char> token;
  for (int read = stream.get(); read != EOF; read = stream.get()) {
    if (std::isdigit(read) || read == '.' || read == '-' || read == '+') {
      token.push_back((char)read);
      continue;
    }

    if (token.size() != 0) {
      auto maybe_float = ParseFloat(token);
      if (!maybe_float) {
        return absl::nullopt;
      }

      result.push_back(*maybe_float);
      token.clear();
    }

    if (read == '#') {
      for (read = stream.get(); read != EOF; read = stream.get()) {
        char ch = static_cast<char>(read);
        if (ch == '\r' || ch == '\n') {
          break;
        }
      }

      continue;
    }

    if (std::isspace(read)) {
      continue;
    }

    return absl::nullopt;
  }

  if (token.size() != 0) {
    auto maybe_float = ParseFloat(token);
    if (!maybe_float) {
      return absl::nullopt;
    }

    result.push_back(*maybe_float);
  }

  return result;
}

absl::optional<std::vector<float_t>> ReadSpdFile(const std::string& search_dir,
                                                 const std::string& file) {
  std::ifstream stream(file);
  if (stream.fail()) {
    std::cerr << "ERROR: Error opening file: " << file << std::endl;
    exit(EXIT_FAILURE);
  }

  return ReadSpdFile(file, stream);
}

}  // namespace iris