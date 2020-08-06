#ifndef _SRC_COMMON_PARAMETER_
#define _SRC_COMMON_PARAMETER_

#include <string>
#include <utility>
#include <vector>

#include "absl/types/optional.h"
#include "absl/types/variant.h"
#include "iris_camera/iris_camera.h"

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

struct UnspacedColorParameter {
  std::vector<std::array<float_t, 3>> data;
};

typedef absl::variant<BoolParameter, IntParameter, FloatParameter,
                      SpectrumParameter, Point3Parameter, Vector3Parameter,
                      NormalParameter, ColorParameter, StringParameter,
                      TextureParameter, UnspacedColorParameter>
    ParameterData;

typedef std::pair<std::string, ParameterData> Parameter;

}  // namespace iris

#endif  // _SRC_COMMON_PARAMETER_