#include "src/param_matchers/file.h"

namespace iris {
namespace {

const size_t variant_indices[1] = {GetIndex<StringParameter, ParameterData>()};

}  // namespace

SingleFileMatcher::SingleFileMatcher(absl::string_view parameter_name)
    : ParameterMatcher(parameter_name, false, variant_indices) {}

const std::pair<std::string, std::string>& SingleFileMatcher::Get() const {
  return m_value;
}

void SingleFileMatcher::Match(ParameterData& data) {
  if (absl::get<StringParameter>(data).data.size() != 1) {
    NumberOfElementsError();
  }
  m_value =
      std::make_pair(std::move(absl::get<StringParameter>(data).data[0]),
                     std::move(absl::get<StringParameter>(data).resolved[0]));
}

}  // namespace iris