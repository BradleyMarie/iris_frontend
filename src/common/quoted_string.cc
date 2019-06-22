#include "src/common/quoted_string.h"

namespace iris {

absl::optional<absl::string_view> UnquoteToken(absl::string_view token) {
  if (token.size() <= 2 || token.front() != '"' || token.back() != '"') {
    return absl::nullopt;
  }

  token.remove_prefix(1);
  token.remove_suffix(1);

  return token;
}

bool ParseQuotedTokenToString(absl::string_view token, std::string* result) {
  auto parsed = UnquoteToken(token);
  if (!parsed) {
    return false;
  }

  result->assign(parsed->data(), parsed->size());

  return true;
}

}  // namespace iris