#include <vector>

#include "absl/types/optional.h"
#include "absl/types/variant.h"
#include "iris/iris.h"
#include "src/tokenizer.h"

namespace iris {

enum ParameterType {
  TYPE_BOOL,
  TYPE_INT,
  TYPE_FLOAT,
  TYPE_POINT,
  TYPE_VECTOR,
  TYPE_NORMAL,
  TYPE_SPECTRUM,
  TYPE_STRING
};

struct Parameter {
  std::string name;
  ParameterType type;
  absl::variant<std::vector<bool>, std::vector<int>, std::vector<float_t>,
                std::vector<POINT3>, std::vector<VECTOR3>,
                std::vector<std::string>>
      data;
};

absl::optional<Parameter> ParseNextParam(Tokenizer& tokenizer);

}  // namespace iris