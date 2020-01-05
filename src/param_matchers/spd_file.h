#ifndef _SRC_PARAM_MATCHERS_SPD_FILE_
#define _SRC_PARAM_MATCHERS_SPD_FILE_

#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace iris {

std::vector<float_t> ReadSpdFile(std::istream& stream);
std::vector<float_t> ReadSpdFile(const std::string& search_dir,
                                 const std::string& file);

}  // namespace iris

#endif  // _SRC_PARAM_MATCHERS_SPD_FILE_