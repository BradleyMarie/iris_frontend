#include "src/materials/parser.h"

#include <iostream>
#include <sstream>

#include "src/common/call_directive.h"
#include "src/materials/matte.h"
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

StringParameter FindType(Tokenizer& tokenizer) {
  for (auto param = ParseNextParam(tokenizer); param;
       param = ParseNextParam(tokenizer)) {
    if (param->first == "type" &&
        absl::holds_alternative<StringParameter>(param->second)) {
      return absl::get<StringParameter>(param->second);
    }
  }

  std::cerr
      << "ERROR: Missing value for required MakeNamedMatarial parameter: type"
      << std::endl;
  exit(EXIT_FAILURE);
}

template <size_t NumImplementations, typename... Args>
Material CallMaterialDirective(
    const char* base_type_name, const std::string& type, Tokenizer& tokenizer,
    const std::array<std::pair<const char*, DirectiveImpl<Material, Args...>>,
                     NumImplementations>& callbacks,
    Args... args) {
  for (auto& entry : callbacks) {
    if (type == entry.first) {
      return entry.second(base_type_name, entry.first, tokenizer, args...);
    }
  }

  std::cerr << "ERROR: Invalid MakeSharedMaterial type specified: " << type
            << std::endl;
  exit(EXIT_FAILURE);
}

const static char* kMatteTypeName = "matte";

}  // namespace

Material ParseMaterial(const char* base_type_name, Tokenizer& tokenizer,
                       MaterialManager& material_manager,
                       const NamedTextureManager& named_texture_manager,
                       TextureManager& texture_manager,
                       SpectrumManager& spectrum_manager) {
  return CallDirective<Material, 1, MaterialManager&,
                       const NamedTextureManager&, TextureManager&,
                       SpectrumManager&, bool>(
      base_type_name, tokenizer, {std::make_pair(kMatteTypeName, ParseMatte)},
      material_manager, named_texture_manager, texture_manager,
      spectrum_manager, false);
}

Material ParseMakeNamedMaterial(
    const char* base_type_name, Tokenizer& tokenizer,
    NamedMaterialManager& named_material_manager,
    MaterialManager& material_manager,
    const NamedTextureManager& named_texture_manager,
    TextureManager& texture_manager, SpectrumManager& spectrum_manager) {
  std::string name(ParseNextQuotedString(base_type_name, tokenizer, "name"));

  std::string tokens;
  for (auto token = tokenizer.Peek(); token; token = tokenizer.Next()) {
    auto unquoted_token = UnquoteToken(*token);
    if (!unquoted_token && *token != "[" && *token != "]") {
      break;
    }
    tokens += ' ';
    tokens.append(token->begin(), token->end());
  }

  std::stringstream token_stream(tokens);
  auto sub_tokenizer = Tokenizer::CreateFromStream(".", token_stream);
  auto type = FindType(tokenizer);

  if (type.data.size() != 1) {
    std::cerr << "ERROR: Wrong number of values for MakeSharedMaterial "
                 "parameter: type"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  token_stream = std::stringstream(std::move(tokens));
  sub_tokenizer = Tokenizer::CreateFromStream(".", token_stream);

  auto material =
      CallMaterialDirective<1, MaterialManager&, const NamedTextureManager&,
                            TextureManager&, SpectrumManager&, bool>(
          base_type_name, type.data[0], tokenizer,
          {std::make_pair(kMatteTypeName, ParseMatte)}, material_manager,
          named_texture_manager, texture_manager, spectrum_manager, true);

  named_material_manager.SetMaterial(name, material);

  return material;
}

Material ParseNamedMaterial(
    const char* base_type_name, Tokenizer& tokenizer,
    const NamedMaterialManager& named_material_manager) {
  auto name = ParseNextQuotedString(base_type_name, tokenizer, "name");
  return named_material_manager.GetMaterial(name);
}

}  // namespace iris