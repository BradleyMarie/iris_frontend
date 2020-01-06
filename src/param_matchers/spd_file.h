#ifndef _SRC_PARAM_MATCHERS_SPD_FILE_
#define _SRC_PARAM_MATCHERS_SPD_FILE_

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "absl/types/optional.h"
#include "src/common/tokenizer.h"

namespace iris {

absl::optional<std::vector<float_t>> ReadSpdFile(const std::string& file,
                                                 std::istream& stream);
absl::optional<std::vector<float_t>> ReadSpdFile(const std::string& search_dir,
                                                 const std::string& file);

}  // namespace iris

#endif  // _SRC_PARAM_MATCHERS_SPD_FILE_