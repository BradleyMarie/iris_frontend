#include "src/param_matchers/parser.h"

#include <iostream>
#include <utility>
#include <vector>

#include "absl/strings/numbers.h"
#include "absl/strings/str_split.h"
#include "src/common/quoted_string.h"
#include "src/param_matchers/flags.h"

namespace iris {
namespace {

static bool ParseStringWithoutValidation(absl::string_view token,
                                         std::string* result) {
  result->assign(token.begin(), token.size());
  return true;
}

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

  result->push_back(std::move(value));
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

static COLOR3 MakeColor(float_t x, float_t y, float_t z) {
  float_t values[3] = {x, y, z};
  return ColorCreate(absl::GetFlag(FLAGS_rgb_color_space), values);
}

static ColorParameter ParseColor(Tokenizer& tokenizer) {
  auto data = ParseFloatTuple<COLOR3, MakeColor, ColorValidate>(
      tokenizer, "Color", "color");
  return ColorParameter{std::move(data)};
}

static ColorParameter ParseRgb(Tokenizer& tokenizer) {
  auto data = ParseFloatTuple<COLOR3, MakeColor, ColorValidate>(tokenizer,
                                                                "Rgb", "rgb");
  return ColorParameter{std::move(data)};
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

static COLOR3 MakeXyz(float_t x, float_t y, float_t z) {
  float_t values[3] = {x, y, z};
  return ColorCreate(COLOR_SPACE_XYZ, values);
}

static ColorParameter ParseXyz(Tokenizer& tokenizer) {
  auto data =
      ParseFloatTuple<COLOR3, MakeXyz, ColorValidate>(tokenizer, "Xyz", "xyz");
  return ColorParameter{std::move(data)};
}

static std::vector<std::pair<std::string, std::string>> ParseSpectrumFilenames(
    const std::vector<std::string>& quoted_filenames,
    const Tokenizer& tokenizer) {
  std::vector<std::string> parsed_filenames;
  for (const auto& filename : quoted_filenames) {
    ParseSingle<std::string, ParseQuotedTokenToString>(filename, "spectrum",
                                                       &parsed_filenames);
  }
  std::vector<std::pair<std::string, std::string>> result;
  for (const auto& filename : parsed_filenames) {
    result.push_back(std::make_pair(filename, tokenizer.ResolvePath(filename)));
  }
  return result;
}

static std::pair<std::vector<std::string>, std::vector<float_t>>
ParseSpectrumSamples(const std::vector<std::string>& samples) {
  std::vector<float_t> result;
  for (const auto& sample : samples) {
    ParseSingle<float_t, absl::SimpleAtof>(sample, "spectrum", &result);
  }
  return std::make_pair(samples, std::move(result));
}

static SpectrumParameter ParseSpectrum(Tokenizer& tokenizer) {
  std::vector<std::string> unknown_data =
      ParseData<std::string, ParseStringWithoutValidation>(
          tokenizer, "Spectrum", "spectrum");
  if (unknown_data.empty() || unknown_data[0][0] == '"') {
    return {{ParseSpectrumFilenames(unknown_data, tokenizer)}};
  } else {
    return {{ParseSpectrumSamples(unknown_data)}};
  }
}

typedef std::function<ParameterData(Tokenizer&)> ParserCallback;

template <typename Result>
ParserCallback ToCallback(std::function<Result(Tokenizer&)> func) {
  return [func](Tokenizer& tokenizer) -> Result { return func(tokenizer); };
}

static const std::map<absl::string_view, ParserCallback> kParsers = {
    // blackbody
    {"bool", ToCallback<BoolParameter>(ParseBool)},
    {"color", ToCallback<ColorParameter>(ParseColor)},
    {"float", ToCallback<FloatParameter>(ParseFloat)},
    {"integer", ToCallback<IntParameter>(ParseInt)},
    {"normal", ToCallback<NormalParameter>(ParseNormal)},
    {"point", ToCallback<Point3Parameter>(ParsePoint)},
    // point2
    {"point3", ToCallback<Point3Parameter>(ParsePoint3)},
    {"rgb", ToCallback<ColorParameter>(ParseRgb)},
    {"spectrum", ToCallback<SpectrumParameter>(ParseSpectrum)},
    {"string", ToCallback<StringParameter>(ParseString)},
    {"texture", ToCallback<TextureParameter>(ParseTexture)},
    {"vector", ToCallback<Vector3Parameter>(ParseVector)},
    // vector2
    {"vector3", ToCallback<Vector3Parameter>(ParseVector3)},
    {"xyz", ToCallback<ColorParameter>(ParseXyz)},
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