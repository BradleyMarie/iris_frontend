#ifndef _SRC_QUOTED_STRING_
#define _SRC_QUOTED_STRING_

#include "absl/strings/string_view.h"
#include "absl/types/optional.h"

namespace iris {

absl::optional<absl::string_view> UnquoteToken(absl::string_view token);
bool ParseQuotedTokenToString(absl::string_view token, std::string* result);

}  // namespace iris

#endif  // _SRC_QUOTED_STRING_