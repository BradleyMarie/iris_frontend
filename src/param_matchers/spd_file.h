#ifndef _SRC_PARAM_MATCHERS_SPD_FILE_
#define _SRC_PARAM_MATCHERS_SPD_FILE_

#include <cmath>
#include <istream>
#include <string>
#include <vector>

#include "absl/types/optional.h"
#include "absl/strings/string_view.h"

namespace iris {

absl::optional<std::vector<float_t>> ReadSpdFile(std::istream& stream);
absl::optional<std::vector<float_t>> ReadSpdFile(
    absl::string_view file, const std::string& resolved_file_path);

}  // namespace iris

#endif  // _SRC_PARAM_MATCHERS_SPD_FILE_