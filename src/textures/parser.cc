#include "src/textures/parser.h"

#include <iostream>

#include "src/common/call_directive.h"
#include "src/textures/constant.h"
#include "src/textures/scale.h"

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

}  // namespace

void ParseTexture(const char* base_type_name, Tokenizer& tokenizer,
                  NamedTextureManager& named_texture_manager,
                  TextureManager& texture_manager,
                  SpectrumManager& spectrum_manager) {
  std::string name(ParseNextQuotedString(base_type_name, tokenizer, "name"));
  absl::string_view format_name =
      ParseNextQuotedString(base_type_name, tokenizer, "format");
  if (format_name == "spectrum" || format_name == "color") {
    auto reflector_texture =
        CallDirective<ReflectorTexture, 2, const NamedTextureManager&,
                      TextureManager&, SpectrumManager&>(
            base_type_name, tokenizer,
            {std::make_pair("constant", ParseConstantReflector),
             std::make_pair("scale", ParseScaleReflector)},
            named_texture_manager, texture_manager, spectrum_manager);
    named_texture_manager.SetReflectorTexture(name, reflector_texture);
    return;
  }

  if (format_name == "float") {
    auto float_texture =
        CallDirective<FloatTexture, 2, const NamedTextureManager&,
                      TextureManager&>(
            base_type_name, tokenizer,
            {std::make_pair("constant", ParseConstantFloat),
             std::make_pair("scale", ParseScaleFloat)},
            named_texture_manager, texture_manager);
    named_texture_manager.SetFloatTexture(name, float_texture);
    return;
  }

  std::cerr << "ERROR: Invalid Texture format specified: " << format_name
            << std::endl;
  exit(EXIT_FAILURE);
}

}  // namespace iris