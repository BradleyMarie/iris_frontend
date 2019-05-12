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

static std::vector<float_t> ParseFloat(Tokenizer& tokenizer) {
  return std::vector<float_t>();
}

static std::vector<int> ParseInt(Tokenizer& tokenizer) {
  return std::vector<int>();
}

static std::vector<bool> ParseBool(Tokenizer& tokenizer) {
  return std::vector<bool>();
}

static std::vector<POINT3> ParsePoint3(Tokenizer& tokenizer) {
  return std::vector<POINT3>();
}

static std::vector<VECTOR3> ParseVector3(Tokenizer& tokenizer) {
  return std::vector<VECTOR3>();
}

static std::vector<std::string> ParseString(Tokenizer& tokenizer) {
  return std::vector<std::string>();
}

static std::vector<float_t> ParseSpectrum(Tokenizer& tokenizer) {
  return std::vector<float_t>();
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
  result.name = std::string(type_and_name[1].data(), type_and_name[1].size());

  if (type_and_name[0] == "float") {
    result.type = TYPE_FLOAT;
    result.data = ParseFloat(tokenizer);
  } else if (type_and_name[0] == "integer") {
    result.type = TYPE_INT;
    result.data = ParseInt(tokenizer);
  } else if (type_and_name[0] == "bool") {
    result.type = TYPE_BOOL;
    result.data = ParseBool(tokenizer);
  } else if (type_and_name[0] == "point2") {
    std::cerr << "ERROR: Illegal parameter type: " << type_and_name[0]
              << std::endl;
    exit(EXIT_FAILURE);
  } else if (type_and_name[0] == "vector2") {
    std::cerr << "ERROR: Illegal parameter type: " << type_and_name[0]
              << std::endl;
    exit(EXIT_FAILURE);
  } else if (type_and_name[0] == "point3") {
    result.type = TYPE_POINT;
    result.data = ParsePoint3(tokenizer);
  } else if (type_and_name[0] == "vector3") {
    result.type = TYPE_VECTOR;
    result.data = ParseVector3(tokenizer);
  } else if (type_and_name[0] == "point") {
    result.type = TYPE_POINT;
    result.data = ParsePoint3(tokenizer);
  } else if (type_and_name[0] == "vector") {
    result.type = TYPE_VECTOR;
    result.data = ParseVector3(tokenizer);
  } else if (type_and_name[0] == "normal") {
    result.type = TYPE_NORMAL;
    result.data = ParseVector3(tokenizer);
  } else if (type_and_name[0] == "string") {
    result.type = TYPE_STRING;
    result.data = ParseString(tokenizer);
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
    result.type = TYPE_SPECTRUM;
    result.data = ParseSpectrum(tokenizer);
  } else {
    std::cerr << "ERROR: Illegal parameter type: " << type_and_name[0]
              << std::endl;
    exit(EXIT_FAILURE);
  }

  return result;
}

}  // namespace iris