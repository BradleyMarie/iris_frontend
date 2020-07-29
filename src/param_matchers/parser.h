#ifndef _SRC_PARAM_MATCHERS_PARSER_
#define _SRC_PARAM_MATCHERS_PARSER_

#include <array>
#include <utility>
#include <vector>

#include "absl/types/optional.h"
#include "absl/types/variant.h"
#include "iris_physx/iris_physx.h"
#include "src/common/pointer_types.h"
#include "src/common/tokenizer.h"

namespace iris {

struct BoolParameter {
  std::vector<bool> data;
};

struct IntParameter {
  std::vector<int> data;
};

struct FloatParameter {
  std::vector<float_t> data;
};

struct SpectrumParameter {
  absl::variant<std::pair<std::vector<std::string>, std::vector<float_t>>,
                std::vector<std::pair<std::string, std::string>>>
      data;
};

struct Point3Parameter {
  std::vector<POINT3> data;
};

struct Vector3Parameter {
  std::vector<VECTOR3> data;
};

struct NormalParameter {
  std::vector<VECTOR3> data;
};

struct ColorParameter {
  std::vector<COLOR3> data;
};

struct StringParameter {
  std::vector<std::string> data;
};

struct TextureParameter {
  std::vector<std::string> data;
};

typedef absl::variant<BoolParameter, IntParameter, FloatParameter,
                      SpectrumParameter, Point3Parameter, Vector3Parameter,
                      NormalParameter, ColorParameter, StringParameter,
                      TextureParameter>
    ParameterData;

typedef std::pair<std::string, ParameterData> Parameter;

absl::optional<Parameter> ParseNextParam(Tokenizer& tokenizer);

}  // namespace iris

#endif  // _SRC_PARAM_MATCHERS_PARSER_