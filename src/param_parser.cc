#include "src/param_parser.h"

#include <iostream>
#include <vector>

#include "absl/strings/str_split.h"

namespace iris {
namespace {

static bool IsQuotedToken(const absl::optional<absl::string_view>& token) {
  if (!token) {
    return false;
  }

  if (token->begin() == token->end()) {
    return false;
  }

  return *token->begin() == '"' && *token->end() == '"';
}

static FloatParameter ParseFloat(Tokenizer& tokenizer) {
  return FloatParameter();
}

static IntParameter ParseInt(Tokenizer& tokenizer) { return IntParameter(); }

static BoolParameter ParseBool(Tokenizer& tokenizer) { return BoolParameter(); }

static Point3Parameter ParsePoint3(Tokenizer& tokenizer) {
  return Point3Parameter();
}

static Vector3Parameter ParseVector3(Tokenizer& tokenizer) {
  return Vector3Parameter();
}

static NormalParameter ParseNormal(Tokenizer& tokenizer) {
  return NormalParameter();
}

static StringParameter ParseString(Tokenizer& tokenizer) {
  return StringParameter();
}

static SpectrumParameter ParseSpectrum(Tokenizer& tokenizer) {
  return SpectrumParameter();
}

}  // namespace

absl::optional<Parameter> ParseNextParam(Tokenizer& tokenizer) {
  if (!IsQuotedToken(tokenizer.Peek())) {
    return absl::nullopt;
  }

  absl::string_view serialized_type_and_name = *tokenizer.Next();
  serialized_type_and_name.remove_prefix(1);
  serialized_type_and_name.remove_suffix(1);

  std::vector<absl::string_view> type_and_name =
      absl::StrSplit(serialized_type_and_name, " ", absl::SkipEmpty());

  if (type_and_name.size() != 2) {
    std::cerr << "ERROR: Failed to parse parameter type and name: "
              << serialized_type_and_name << std::endl;
    exit(EXIT_FAILURE);
  }

  Parameter result;
  result.first = std::string(type_and_name[1].data(), type_and_name[1].size());

  if (type_and_name[0] == "float") {
    result.second = ParseFloat(tokenizer);
  } else if (type_and_name[0] == "integer") {
    result.second = ParseInt(tokenizer);
  } else if (type_and_name[0] == "bool") {
    result.second = ParseBool(tokenizer);
  } else if (type_and_name[0] == "point2") {
    std::cerr << "ERROR: Illegal parameter type: " << type_and_name[0]
              << std::endl;
    exit(EXIT_FAILURE);
  } else if (type_and_name[0] == "vector2") {
    std::cerr << "ERROR: Illegal parameter type: " << type_and_name[0]
              << std::endl;
    exit(EXIT_FAILURE);
  } else if (type_and_name[0] == "point3") {
    result.second = ParsePoint3(tokenizer);
  } else if (type_and_name[0] == "vector3") {
    result.second = ParseVector3(tokenizer);
  } else if (type_and_name[0] == "point") {
    result.second = ParsePoint3(tokenizer);
  } else if (type_and_name[0] == "vector") {
    result.second = ParseVector3(tokenizer);
  } else if (type_and_name[0] == "normal") {
    result.second = ParseNormal(tokenizer);
  } else if (type_and_name[0] == "string") {
    result.second = ParseString(tokenizer);
  } else if (type_and_name[0] == "texture") {
    std::cerr << "ERROR: Illegal parameter type: " << type_and_name[0]
              << std::endl;
    exit(EXIT_FAILURE);
  } else if (type_and_name[0] == "color") {
    std::cerr << "ERROR: Illegal parameter type: " << type_and_name[0]
              << std::endl;
    exit(EXIT_FAILURE);
  } else if (type_and_name[0] == "rgb") {
    std::cerr << "ERROR: Illegal parameter type: " << type_and_name[0]
              << std::endl;
    exit(EXIT_FAILURE);
  } else if (type_and_name[0] == "xyz") {
    std::cerr << "ERROR: Illegal parameter type: " << type_and_name[0]
              << std::endl;
    exit(EXIT_FAILURE);
  } else if (type_and_name[0] == "blackbody") {
    std::cerr << "ERROR: Illegal parameter type: " << type_and_name[0]
              << std::endl;
    exit(EXIT_FAILURE);
  } else if (type_and_name[0] == "spectrum") {
    result.second = ParseSpectrum(tokenizer);
  } else {
    std::cerr << "ERROR: Illegal parameter type: " << type_and_name[0]
              << std::endl;
    exit(EXIT_FAILURE);
  }

  return result;
}

}  // namespace iris