#ifndef _SRC_PARAM_MATCHERS_SPD_FILE_
#define _SRC_PARAM_MATCHERS_SPD_FILE_

#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "src/common/tokenizer.h"

namespace iris {

void InvalidSpdFile [[noreturn]] (absl::string_view filename);
std::vector<float_t> ReadSpdFile(const std::string& file, std::istream& stream);
std::vector<float_t> ReadSpdFile(const Tokenizer& tokenizer,
                                 const std::string& file);

}  // namespace iris

#endif  // _SRC_PARAM_MATCHERS_SPD_FILE_