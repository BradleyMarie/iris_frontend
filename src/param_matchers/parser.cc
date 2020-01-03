#include "src/param_matchers/parser.h"

#include <iostream>
#include <vector>

#include "absl/strings/numbers.h"
#include "absl/strings/str_join.h"
#include "absl/strings/str_split.h"
#include "iris_physx_toolkit/interpolated_spectrum.h"
#include "src/common/error.h"
#include "src/common/quoted_string.h"

namespace iris {
namespace {

static bool ParseQuotedTokenToBool(absl::string_view token, bool* result) {
  if (token == "\"true\"") {
    *result = true;
    return true;
  }

  if (token == "\"false\"") {
    *result = false;
    return true;
  }

  return false;
}

template <typename Type, bool (*ParseFunc)(absl::string_view, Type*)>
void ParseSingle(absl::string_view token, const char* lower_type_name,
                 std::vector<Type>* result) {
  Type value;
  bool success = ParseFunc(token, &value);

  if (!success) {
    std::cerr << "ERROR: Failed to parse " << lower_type_name
              << " parameter: " << token << std::endl;
    exit(EXIT_FAILURE);
  }

  result->push_back(value);
}

template <typename Type, bool (*ParseFunc)(absl::string_view, Type*)>
void ParseLoop(Tokenizer& tokenizer, const char* type_name,
               const char* lower_type_name, std::vector<Type>* result) {
  for (;;) {
    auto token = tokenizer.Next();

    if (!token) {
      std::cerr << "ERROR: " << type_name
                << " parameter arrays must end with ']'" << std::endl;
      exit(EXIT_FAILURE);
    }

    if (*token == "]") {
      break;
    }

    ParseSingle<Type, ParseFunc>(*token, lower_type_name, result);
  }
}

template <typename Type, bool (*ParseFunc)(absl::string_view, Type*)>
std::vector<Type> ParseData(Tokenizer& tokenizer, const char* type_name,
                            const char* lower_type_name) {
  auto token = *tokenizer.Next();

  std::vector<Type> result;
  if (token != "[") {
    ParseSingle<Type, ParseFunc>(token, lower_type_name, &result);
  } else {
    ParseLoop<Type, ParseFunc>(tokenizer, type_name, lower_type_name, &result);
  }

  return result;
}

static FloatParameter ParseFloat(Tokenizer& tokenizer) {
  auto data = ParseData<float_t, absl::SimpleAtof>(tokenizer, "Float", "float");
  return FloatParameter{std::move(data)};
}

static IntParameter ParseInt(Tokenizer& tokenizer) {
  auto data = ParseData<int, absl::SimpleAtoi>(tokenizer, "Int", "int");
  return IntParameter{std::move(data)};
}

static BoolParameter ParseBool(Tokenizer& tokenizer) {
  auto data =
      ParseData<bool, ParseQuotedTokenToBool>(tokenizer, "Bool", "bool");
  return BoolParameter{std::move(data)};
}

template <typename Type, Type (*Create)(float_t x, float_t y, float_t z),
          bool (*Validate)(Type)>
static std::vector<Type> ParseFloatTuple(Tokenizer& tokenizer,
                                         const char* type_name,
                                         const char* lower_type_name) {
  auto data = ParseData<float_t, absl::SimpleAtof>(tokenizer, type_name,
                                                   lower_type_name);
  if (data.size() % 3 != 0) {
    std::cerr << "ERROR: The number of parameters for " << lower_type_name
              << " must be divisible by 3" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::vector<Type> result;
  for (size_t i = 0; i < data.size(); i += 3) {
    auto value = Create(data[i], data[i + 1], data[i + 2]);
    if (!Validate(value)) {
      std::cerr << "ERROR: Could not construct a " << lower_type_name
                << " from values (" << data[i] << ", " << data[i + 1] << ", "
                << data[i + 2] << ")" << std::endl;
      exit(EXIT_FAILURE);
    }

    result.push_back(value);
  }

  return result;
}

static Point3Parameter ParsePoint(Tokenizer& tokenizer) {
  auto data = ParseFloatTuple<POINT3, PointCreate, PointValidate>(
      tokenizer, "Point", "point");
  return Point3Parameter{std::move(data)};
}

static Point3Parameter ParsePoint3(Tokenizer& tokenizer) {
  auto data = ParseFloatTuple<POINT3, PointCreate, PointValidate>(
      tokenizer, "Point3", "point3");
  return Point3Parameter{std::move(data)};
}

static Vector3Parameter ParseVector(Tokenizer& tokenizer) {
  auto data = ParseFloatTuple<VECTOR3, VectorCreate, VectorValidate>(
      tokenizer, "Vector", "vector");
  return Vector3Parameter{std::move(data)};
}

static Vector3Parameter ParseVector3(Tokenizer& tokenizer) {
  auto data = ParseFloatTuple<VECTOR3, VectorCreate, VectorValidate>(
      tokenizer, "Vector3", "vector3");
  return Vector3Parameter{std::move(data)};
}

static NormalParameter ParseNormal(Tokenizer& tokenizer) {
  auto data = ParseFloatTuple<VECTOR3, VectorCreate, VectorValidate>(
      tokenizer, "Normal", "normal");
  return NormalParameter{std::move(data)};
}

static std::array<float_t, 3> MakeRgb(float_t x, float_t y, float_t z) {
  return {x, y, z};
}

static bool ValidateRgb(std::array<float_t, 3> value) {
  if (!isfinite(value[0]) || value[0] < (float_t)0.0 || !isfinite(value[1]) ||
      value[1] < (float_t)0.0 || !isfinite(value[2]) ||
      value[2] < (float_t)0.0) {
    return false;
  }
  return true;
}

static RgbParameter ParseColor(Tokenizer& tokenizer) {
  auto data = ParseFloatTuple<std::array<float_t, 3>, MakeRgb, ValidateRgb>(
      tokenizer, "Color", "color");
  return RgbParameter{std::move(data)};
}

static RgbParameter ParseRgb(Tokenizer& tokenizer) {
  auto data = ParseFloatTuple<std::array<float_t, 3>, MakeRgb, ValidateRgb>(
      tokenizer, "Rgb", "rgb");
  return RgbParameter{std::move(data)};
}

static StringParameter ParseString(Tokenizer& tokenizer) {
  auto data = ParseData<std::string, ParseQuotedTokenToString>(
      tokenizer, "String", "string");
  return StringParameter{std::move(data)};
}

static TextureParameter ParseTexture(Tokenizer& tokenizer) {
  auto data = ParseData<std::string, ParseQuotedTokenToString>(
      tokenizer, "Texture", "texture");
  return TextureParameter{std::move(data)};
}

static XyzParameter ParseXyz(Tokenizer& tokenizer) {
  auto data = ParseFloatTuple<COLOR3, ColorCreate, ColorValidate>(tokenizer,
                                                                  "Xyz", "xyz");
  return XyzParameter{std::move(data)};
}

template <typename ContainerType, typename PointerType,
          ISTATUS (*Allocate)(const float_t*, const float_t*, size_t,
                              PointerType*)>
static ContainerType AllocateSpectrum(const std::vector<float_t>& data,
                                      const std::vector<float_t>& wavelengths,
                                      const std::vector<float_t>& intensities) {
  ContainerType result;
  ISTATUS status =
      Allocate(wavelengths.data(), intensities.data(), wavelengths.size(),
               result.release_and_get_address());
  if (status == ISTATUS_ALLOCATION_FAILED) {
    ReportOOM();
  }
  if (status != ISTATUS_SUCCESS) {
    std::cerr << "ERROR: Could not construct a spectrum from values ("
              << absl::StrJoin(data, ", ") << ")" << std::endl;
    exit(EXIT_FAILURE);
  }

  return result;
}

static SpectrumParameter ParseSpectrum(Tokenizer& tokenizer) {
  if (UnquoteToken(*tokenizer.Peek())) {
    std::cerr << "ERROR: String values are unsupported for Spectrum parameters"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  auto data =
      ParseData<float_t, absl::SimpleAtof>(tokenizer, "Spectrum", "spectrum");
  if (data.size() % 2 != 0) {
    std::cerr << "ERROR: The number of parameters for spectrum must be "
                 "divisible by 2"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  bool allocate_reflector = true;
  std::vector<float_t> wavelengths;
  std::vector<float_t> intensities;
  for (size_t i = 0; i < data.size(); i += 2) {
    wavelengths.push_back(data[i]);
    intensities.push_back(data[i + 1]);

    if ((float_t)1.0 < data[i + 1]) {
      allocate_reflector = false;
    }
  }

  Spectrum spectrum =
      AllocateSpectrum<Spectrum, PSPECTRUM, InterpolatedSpectrumAllocate>(
          data, wavelengths, intensities);

  Reflector reflector;
  if (allocate_reflector) {
    reflector =
        AllocateSpectrum<Reflector, PREFLECTOR, InterpolatedReflectorAllocate>(
            data, wavelengths, intensities);
  }

  std::vector<std::pair<Spectrum, Reflector>> result;
  result.push_back(std::make_pair(spectrum, reflector));
  std::vector<std::vector<float_t>> values;
  values.push_back(std::move(data));
  return SpectrumParameter{std::move(result), std::move(values)};
}

typedef std::function<ParameterData(Tokenizer&)> ParserCallback;

template <typename Result>
ParserCallback ToCallback(std::function<Result(Tokenizer&)> func) {
  return [func](Tokenizer& tokenizer) -> Result { return func(tokenizer); };
}

static const std::map<absl::string_view, ParserCallback> kParsers = {
    // blackbody
    {"bool", ToCallback<BoolParameter>(ParseBool)},
    {"color", ToCallback<RgbParameter>(ParseColor)},
    {"float", ToCallback<FloatParameter>(ParseFloat)},
    {"integer", ToCallback<IntParameter>(ParseInt)},
    {"normal", ToCallback<NormalParameter>(ParseNormal)},
    {"point", ToCallback<Point3Parameter>(ParsePoint)},
    // point2
    {"point3", ToCallback<Point3Parameter>(ParsePoint3)},
    {"rgb", ToCallback<RgbParameter>(ParseRgb)},
    {"spectrum", ToCallback<SpectrumParameter>(ParseSpectrum)},
    {"string", ToCallback<StringParameter>(ParseString)},
    {"texture", ToCallback<TextureParameter>(ParseTexture)},
    {"vector", ToCallback<Vector3Parameter>(ParseVector)},
    // vector2
    {"vector3", ToCallback<Vector3Parameter>(ParseVector3)},
    {"xyz", ToCallback<XyzParameter>(ParseXyz)},
};

}  // namespace

absl::optional<Parameter> ParseNextParam(Tokenizer& tokenizer) {
  auto quoted_token = tokenizer.Peek();
  if (!quoted_token) {
    return absl::nullopt;
  }

  auto unquoted_token = UnquoteToken(*quoted_token);
  if (!unquoted_token) {
    return absl::nullopt;
  }

  std::vector<absl::string_view> type_and_name =
      absl::StrSplit(*unquoted_token, " ", absl::SkipEmpty());

  if (type_and_name.size() != 2) {
    std::cerr << "ERROR: Failed to parse parameter type and name: "
              << *quoted_token << std::endl;
    exit(EXIT_FAILURE);
  }

  auto parser = kParsers.find(type_and_name[0]);
  if (parser == kParsers.end()) {
    std::cerr << "ERROR: Illegal parameter type: " << type_and_name[0]
              << std::endl;
    exit(EXIT_FAILURE);
  }

  std::string name(type_and_name[1].data(), type_and_name[1].size());

  quoted_token = tokenizer.Next();
  if (!tokenizer.Peek()) {
    std::cerr << "ERROR: No parameters found for parameter: " << *quoted_token
              << std::endl;
    exit(EXIT_FAILURE);
  }

  return Parameter(std::move(name), parser->second(tokenizer));
}

}  // namespace iris