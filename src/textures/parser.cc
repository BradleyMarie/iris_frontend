#include "src/textures/parser.h"

#include "src/textures/constant.h"
#include "src/textures/imagemap.h"
#include "src/textures/scale.h"

namespace iris {
namespace {

const Directive::Implementations<ReflectorTexture, const NamedTextureManager&,
                                 TextureManager&, SpectrumManager&>
    kReflectorImpls = {{"constant", ParseConstantReflector},
                       {"imagemap", ParseImageMapReflector},
                       {"scale", ParseScaleReflector}};

const Directive::Implementations<FloatTexture, const NamedTextureManager&,
                                 TextureManager&>
    kFloatImpls = {{"constant", ParseConstantFloat},
                   {"imagemap", ParseImageMapFloat},
                   {"scale", ParseScaleFloat}};

}  // namespace

void ParseTexture(Directive& directive,
                  NamedTextureManager& named_texture_manager,
                  TextureManager& texture_manager,
                  SpectrumManager& spectrum_manager) {
  const NamedTextureManager& const_named_texture_manager =
      named_texture_manager;
  auto float_result = directive.TryInvokeNamedWithFormat(
      "float", kFloatImpls, const_named_texture_manager, texture_manager);
  if (float_result.has_value()) {
    named_texture_manager.SetFloatTexture(float_result->first,
                                          float_result->second);
    return;
  }

  auto reflector_texture = directive.TryInvokeNamedWithFormat(
      "color", kReflectorImpls, const_named_texture_manager, texture_manager,
      spectrum_manager);
  if (reflector_texture.has_value()) {
    named_texture_manager.SetReflectorTexture(reflector_texture->first,
                                              reflector_texture->second);
    return;
  }

  reflector_texture = directive.TryInvokeNamedWithFormat(
      "spectrum", kReflectorImpls, const_named_texture_manager, texture_manager,
      spectrum_manager);
  if (reflector_texture.has_value()) {
    named_texture_manager.SetReflectorTexture(reflector_texture->first,
                                              reflector_texture->second);
    return;
  }

  directive.FormatError();
}

}  // namespace iris