#include "src/materials/parser.h"

#include <iostream>

#include "src/common/call_directive.h"
#include "src/materials/matte.h"
#include "src/materials/plastic.h"
#include "src/param_matchers/parser.h"

namespace iris {
namespace {

absl::string_view ParseNextQuotedString(const char* base_type_name,
                                        Tokenizer& tokenizer,
                                        absl::string_view element_name) {
  auto token = tokenizer.Next();
  if (!token) {
    std::cerr << "ERROR: " << base_type_name << " " << element_name
              << " not specified" << std::endl;
    exit(EXIT_FAILURE);
  }

  auto unquoted = UnquoteToken(*token);
  if (!unquoted) {
    std::cerr << "ERROR: Invalid " << base_type_name << " " << element_name
              << " specified: " << *token << std::endl;
    exit(EXIT_FAILURE);
  }

  return *unquoted;
}

const static char* kTypeParameterName = "type";
const static char* kMatteTypeName = "matte";
const static char* kPlasticTypeName = "plastic";

typedef std::function<MaterialFactory(
    const char*, const char*, const Tokenizer&, absl::Span<Parameter>,
    const NamedTextureManager&, NormalMapManager&, TextureManager&,
    SpectrumManager&)>
    MakeNamedMaterialFunction;

std::pair<const char*, MakeNamedMaterialFunction> ParseMaterialType(
    const Parameter& param) {
  if (!absl::holds_alternative<StringParameter>(param.second)) {
    std::cerr << "ERROR: Wrong type for MakeNamedMaterial: "
              << kTypeParameterName << std::endl;
    exit(EXIT_FAILURE);
  }

  if (absl::get<StringParameter>(param.second).data.size() != 1) {
    std::cerr
        << "ERROR: Wrong number of values for MakeNamedMaterial parameter: "
        << kTypeParameterName << std::endl;
    exit(EXIT_FAILURE);
  }

  if (absl::get<StringParameter>(param.second).data[0] == kMatteTypeName) {
    return std::make_pair(kMatteTypeName, MakeNamedMatte);
  }

  if (absl::get<StringParameter>(param.second).data[0] == kPlasticTypeName) {
    return std::make_pair(kPlasticTypeName, MakeNamedPlastic);
  }

  std::cerr << "ERROR: Invalid MakeNamedMaterial type specified: "
            << param.first << std::endl;
  exit(EXIT_FAILURE);
}

}  // namespace

MaterialFactory ParseMaterial(const char* base_type_name, Tokenizer& tokenizer,
                              const NamedTextureManager& named_texture_manager,
                              NormalMapManager& normal_map_manager,
                              TextureManager& texture_manager,
                              SpectrumManager& spectrum_manager) {
  return CallDirective<MaterialFactory, 2, const NamedTextureManager&,
                       NormalMapManager&, TextureManager&, SpectrumManager&>(
      base_type_name, tokenizer,
      {std::make_pair(kMatteTypeName, ParseMatte),
       std::make_pair(kPlasticTypeName, ParsePlastic)},
      named_texture_manager, normal_map_manager, texture_manager,
      spectrum_manager);
}

MaterialFactory ParseMakeNamedMaterial(
    const char* base_type_name, Tokenizer& tokenizer,
    NamedMaterialManager& named_material_manager,
    const NamedTextureManager& named_texture_manager,
    NormalMapManager& normal_map_manager, TextureManager& texture_manager,
    SpectrumManager& spectrum_manager) {
  std::string name(ParseNextQuotedString(base_type_name, tokenizer, "name"));

  std::vector<Parameter> parameters;
  std::pair<const char*, MakeNamedMaterialFunction> name_and_function;
  for (auto param = ParseNextParam(tokenizer); param;
       param = ParseNextParam(tokenizer)) {
    if (param->first == kTypeParameterName) {
      name_and_function = ParseMaterialType(*param);
      continue;
    }
    parameters.push_back(*param);
  }

  if (!name_and_function.second) {
    std::cerr << "ERROR: Missing value for required " << base_type_name
              << " parameter: " << kTypeParameterName << std::endl;
    exit(EXIT_FAILURE);
  }

  auto material = name_and_function.second(
      base_type_name, name_and_function.first, tokenizer,
      absl::MakeSpan(parameters), named_texture_manager, normal_map_manager,
      texture_manager, spectrum_manager);

  named_material_manager.SetMaterial(name, material);

  return material;
}

MaterialFactory ParseNamedMaterial(
    const char* base_type_name, Tokenizer& tokenizer,
    const NamedMaterialManager& named_material_manager) {
  auto name = ParseNextQuotedString(base_type_name, tokenizer, "name");
  return named_material_manager.GetMaterial(name);
}

}  // namespace iris