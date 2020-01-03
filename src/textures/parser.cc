#include "src/textures/parser.h"

#include <iostream>

#include "src/common/call_directive.h"
#include "src/textures/constant.h"

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
                  const ColorExtrapolator& color_extrapolator,
                  TextureManager& texture_manager) {
  std::string name(ParseNextQuotedString(base_type_name, tokenizer, "name"));
  absl::string_view format_name =
      ParseNextQuotedString(base_type_name, tokenizer, "format");
  if (format_name == "spectrum" || format_name == "color") {
    auto reflector_texture =
        CallDirective<std::pair<ReflectorTexture, std::set<Reflector>>, 1,
                      const ColorExtrapolator&, const TextureManager&>(
            base_type_name, tokenizer,
            {std::make_pair("constant", ParseConstantReflector)},
            color_extrapolator, texture_manager);
    texture_manager.SetReflectorTexture(name, reflector_texture.first,
                                        reflector_texture.second);
    return;
  }

  if (format_name == "float") {
    auto float_texture =
        CallDirective<FloatTexture, 1, const TextureManager&>(
            base_type_name, tokenizer,
            {std::make_pair("constant", ParseConstantFloat)},
            texture_manager);
    texture_manager.SetFloatTexture(name, float_texture);
    return;
  }

  std::cerr << "ERROR: Invalid Texture format specified: " << format_name
            << std::endl;
  exit(EXIT_FAILURE);
}

}  // namespace iris