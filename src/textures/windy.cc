#include "src/textures/windy.h"

#include "src/param_matchers/float_texture.h"
#include "src/param_matchers/reflector_texture.h"

namespace iris {
namespace {

static const float_t kWindyTextureDefaultValue = (float_t)1.0;

}  // namespace

ReflectorTexture ParseWindyReflector(
    Parameters& parameters, const Matrix& texture_to_world,
    const NamedTextureManager& named_texture_manager,
    TextureManager& texture_manager, SpectrumManager& spectrum_manager) {
  parameters.Match();

  Reflector reflector =
      spectrum_manager.AllocateUniformReflector((float_t)1.0).value();

  return texture_manager.AllocateWindyReflectorTexture(texture_to_world,
                                                       reflector);
}

FloatTexture ParseWindyFloat(Parameters& parameters,
                             const Matrix& texture_to_world,
                             const NamedTextureManager& named_texture_manager,
                             TextureManager& texture_manager) {
  parameters.Match();

  return texture_manager.AllocateWindyFloatTexture(texture_to_world);
}

}  // namespace iris