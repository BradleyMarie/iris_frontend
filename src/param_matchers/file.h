#ifndef _SRC_PARAM_MATCHER_FILE_
#define _SRC_PARAM_MATCHER_FILE_

#include "src/common/parameter_matcher.h"

namespace iris {

class SingleFileMatcher : public ParameterMatcher {
 public:
  SingleFileMatcher(absl::string_view parameter_name);
  const std::pair<std::string, std::string>& Get() const;

 protected:
  void Match(ParameterData& data) final;

 private:
  std::pair<std::string, std::string> m_value;
};

}  // namespace iris

#endif  // _SRC_PARAM_MATCHER_FILE_