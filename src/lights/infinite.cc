#include "src/lights/infinite.h"

#include <iostream>

#include "absl/strings/match.h"
#include "iris_advanced_toolkit/color_io.h"
#include "iris_advanced_toolkit/lanczos_upscale.h"
#include "iris_physx_toolkit/infinite_environmental_light.h"
#include "src/common/error.h"
#include "src/param_matchers/file.h"
#include "tinyexr.h"

namespace iris {
namespace {

SpectrumMipmap LoadSpectrumMipmapFromExr(
    const char* filename, ColorExtrapolator& color_extrapolator) {
  int width, height;
  float* rgba;
  int success = LoadEXR(&rgba, &width, &height, filename, nullptr);
  if (success != TINYEXR_SUCCESS) {
    std::cerr << "ERROR: Failed to read EXR file: " << filename << std::endl;
    exit(EXIT_FAILURE);
  }

  PCOLOR3 colors = (PCOLOR3)calloc(width * height, sizeof(COLOR3));
  if (colors == nullptr) {
    ReportOOM();
  }

  for (size_t i = 0; i < (size_t)width * (size_t)height; i++) {
    colors[i] = ColorCreate(COLOR_SPACE_LINEAR_SRGB, rgba + (4 * i));
  }

  size_t new_x, new_y;
  ISTATUS status = LanczosUpscaleColors(colors, (size_t)width, (size_t)height,
                                        &colors, &new_x, &new_y);

  if (status != ISTATUS_SUCCESS) {
    free(colors);
    ReportOOM();
  }

  SpectrumMipmap mipmap;
  status = SpectrumMipmapAllocate(
      colors, new_x, new_y, TEXTURE_FILTERING_ALGORITHM_NONE, (float_t)8.0,
      WRAP_MODE_REPEAT, color_extrapolator.get(),
      mipmap.release_and_get_address());
  free(colors);

  SuccessOrOOM(status);

  return mipmap;
}

}  // namespace

LightResult ParseInfinite(Parameters& parameters,
                          SpectrumManager& spectrum_manager,
                          const Matrix& model_to_world,
                          const ColorIntegrator& color_integrator) {
  SingleFileMatcher mapname("mapname");
  parameters.Match(mapname);

  if (!absl::EndsWith(mapname.Get().first, ".exr")) {
    std::cerr << "ERROR: exr is the only supported infinite light image format"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  SpectrumMipmap mipmap = LoadSpectrumMipmapFromExr(
      mapname.Get().second.c_str(), spectrum_manager.GetColorExtrapolator());

  Light light;
  EnvironmentalLight environmental_light;
  ISTATUS status = InfiniteEnvironmentalLightAllocate(
      mipmap.detach(), model_to_world.get(), color_integrator.get(),
      environmental_light.release_and_get_address(),
      light.release_and_get_address());
  SuccessOrOOM(status);

  return std::make_tuple(light, environmental_light);
}

}  // namespace iris