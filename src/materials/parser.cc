#include "src/materials/parser.h"

#include <iostream>

#include "src/common/call_directive.h"
#include "src/materials/matte.h"
#include "src/materials/plastic.h"
#include "src/param_matchers/single.h"

namespace iris {
namespace {

absl::string_view ParseNextQuotedString(absl::string_view base_type_name,
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

absl::string_view kMatteTypeName = "matte";
absl::string_view kPlasticTypeName = "plastic";

}  // namespace

MaterialResult ParseMaterial(absl::string_view base_type_name,
                             Tokenizer& tokenizer,
                             const NamedTextureManager& named_texture_manager,
                             NormalMapManager& normal_map_manager,
                             TextureManager& texture_manager,
                             SpectrumManager& spectrum_manager) {
  auto result =
      CallDirective<MaterialResult, const NamedTextureManager&,
                    NormalMapManager&, TextureManager&, SpectrumManager&>(
          base_type_name, tokenizer,
          {{kMatteTypeName, ParseMatte}, {kPlasticTypeName, ParsePlastic}},
          named_texture_manager, normal_map_manager, texture_manager,
          spectrum_manager);

  return result;
}

std::pair<std::string, MaterialResult> ParseMakeNamedMaterial(
    absl::string_view base_type_name, Tokenizer& tokenizer,
    const NamedTextureManager& named_texture_manager,
    NormalMapManager& normal_map_manager, TextureManager& texture_manager,
    SpectrumManager& spectrum_manager) {
  std::string name(ParseNextQuotedString(base_type_name, tokenizer, "name"));

  SingleStringMatcher type("type", true, std::string());
  Parameters parameters(base_type_name, tokenizer);
  auto unused_params = parameters.MatchAllowUnused(type);

  MaterialResult result;
  if (type.Get() == kMatteTypeName) {
    result = ParseMatte(unused_params, named_texture_manager,
                        normal_map_manager, texture_manager, spectrum_manager);
  } else if (type.Get() == kPlasticTypeName) {
    result =
        ParsePlastic(unused_params, named_texture_manager, normal_map_manager,
                     texture_manager, spectrum_manager);
  } else {
    std::cerr << "ERROR: Invalid MakeNamedMaterial type specified: "
              << type.Get() << std::endl;
    exit(EXIT_FAILURE);
  }

  return std::make_pair(name, result);
}

std::string ParseNamedMaterial(
    absl::string_view base_type_name, Tokenizer& tokenizer) {
  return std::string(ParseNextQuotedString(base_type_name, tokenizer, "name"));
}

}  // namespace iris