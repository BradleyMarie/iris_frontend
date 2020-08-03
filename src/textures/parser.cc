#include "src/textures/parser.h"

#include "src/textures/constant.h"
#include "src/textures/imagemap.h"
#include "src/textures/scale.h"

namespace iris {
namespace {

std::function<void(Parameters&, const std::string&, const NamedTextureManager&,
                   TextureManager&, SpectrumManager&)>
WrapFunction(ReflectorTexture (*function)(Parameters&,
                                          const NamedTextureManager&,
                                          TextureManager&, SpectrumManager&),
             NamedTextureManager& named_texture_manager) {
  return
      [&](Parameters& parameters, const std::string& name,
          const NamedTextureManager& const_named_texture_manager,
          TextureManager& texture_manager, SpectrumManager& spectrum_manager) {
        auto result = function(parameters, const_named_texture_manager,
                               texture_manager, spectrum_manager);
        named_texture_manager.SetReflectorTexture(name, result);
      };
}

std::function<void(Parameters&, const std::string&, const NamedTextureManager&,
                   TextureManager&, SpectrumManager&)>
WrapFunction(FloatTexture (*function)(Parameters&, const NamedTextureManager&,
                                      TextureManager&),
             NamedTextureManager& named_texture_manager) {
  return
      [&](Parameters& parameters, const std::string& name,
          const NamedTextureManager& const_named_texture_manager,
          TextureManager& texture_manager, SpectrumManager& spectrum_manager) {
        auto result =
            function(parameters, const_named_texture_manager, texture_manager);
        named_texture_manager.SetFloatTexture(name, result);
      };
}

}  // namespace

void ParseTexture(Directive& directive,
                  NamedTextureManager& named_texture_manager,
                  TextureManager& texture_manager,
                  SpectrumManager& spectrum_manager) {
  Directive::Implementations<void, const std::string&,
                             const NamedTextureManager&, TextureManager&,
                             SpectrumManager&>
      kReflectorImpls = {
          {"constant",
           WrapFunction(ParseConstantReflector, named_texture_manager)},
          {"imagemap",
           WrapFunction(ParseImageMapReflector, named_texture_manager)},
          {"scale", WrapFunction(ParseScaleReflector, named_texture_manager)}};

  Directive::Implementations<void, const std::string&,
                             const NamedTextureManager&, TextureManager&,
                             SpectrumManager&>
      kFloatImpls = {
          {"constant", WrapFunction(ParseConstantFloat, named_texture_manager)},
          {"imagemap", WrapFunction(ParseImageMapFloat, named_texture_manager)},
          {"scale", WrapFunction(ParseScaleFloat, named_texture_manager)}};

  Directive::AllFormattedImplementations<const NamedTextureManager&,
                                         TextureManager&, SpectrumManager&>
      kImpls = {{"float", kFloatImpls},
                {"color", kReflectorImpls},
                {"spectrum", kReflectorImpls}};

  const NamedTextureManager& c_named_texture_manager = named_texture_manager;
  directive.InvokeNamedMultiFormat(kImpls, c_named_texture_manager,
                                   texture_manager, spectrum_manager);
}

}  // namespace iris