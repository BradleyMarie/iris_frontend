#include "src/textures/imagemap.h"

#include <iostream>
#include <limits>

#include "absl/strings/match.h"
#include "iris_physx_toolkit/png_mipmap.h"
#include "src/param_matchers/file.h"
#include "src/param_matchers/float_single.h"
#include "src/param_matchers/float_texture.h"
#include "src/param_matchers/reflector_texture.h"
#include "src/param_matchers/single.h"

namespace iris {
namespace {

static const bool kImageMapDefaultTrilinear = false;
static const float_t kImageMapDefaultMaxAnisotropy = (float_t)8.0;
static const float_t kImageMapDefaultUVScaleValue = (float_t)1.0;
static const float_t kImageMapDefaultUVDeltaValue = (float_t)0.0;
static const char* kImageMapDefaultWrap = "repeat";

}  // namespace

ReflectorTexture ParseImageMapReflector(
    Parameters& parameters, const Matrix& texture_to_world,
    const NamedTextureManager& named_texture_manager,
    TextureManager& texture_manager, SpectrumManager& spectrum_manager) {
  SingleBoolMatcher trilinear("trilinear", false, kImageMapDefaultTrilinear);
  SingleFloatMatcher maxanisotropy("maxanisotropy", false, false, (float_t)0.0,
                                   std::numeric_limits<float_t>::infinity(),
                                   kImageMapDefaultMaxAnisotropy);
  SingleFloatMatcher u_scale(
      "uscale", false, false, -std::numeric_limits<float_t>::infinity(),
      std::numeric_limits<float_t>::infinity(), kImageMapDefaultUVScaleValue);
  SingleFloatMatcher v_scale(
      "vscale", false, false, -std::numeric_limits<float_t>::infinity(),
      std::numeric_limits<float_t>::infinity(), kImageMapDefaultUVScaleValue);
  SingleFloatMatcher u_delta(
      "udelta", false, false, -std::numeric_limits<float_t>::infinity(),
      std::numeric_limits<float_t>::infinity(), kImageMapDefaultUVDeltaValue);
  SingleFloatMatcher v_delta(
      "vdelta", false, false, -std::numeric_limits<float_t>::infinity(),
      std::numeric_limits<float_t>::infinity(), kImageMapDefaultUVDeltaValue);
  SingleFileMatcher filename("filename");
  SingleStringMatcher wrap("wrap", false, kImageMapDefaultWrap);
  parameters.Match(trilinear, maxanisotropy, u_scale, v_scale, u_delta, v_delta,
                   filename, wrap);

  if (!absl::EndsWith(filename.Get().first, ".png")) {
    std::cerr << "ERROR: png is the only supported image format" << std::endl;
    exit(EXIT_FAILURE);
  }

  WRAP_MODE wrap_mode;
  if (wrap.Get() == "repeat") {
    wrap_mode = WRAP_MODE_REPEAT;
  } else if (wrap.Get() == "black") {
    wrap_mode = WRAP_MODE_BLACK;
  } else if (wrap.Get() == "clamp") {
    wrap_mode = WRAP_MODE_CLAMP;
  } else {
    std::cerr << "ERROR: Unsupported wrap mode for ImageMap: " << wrap.Get()
              << std::endl;
    exit(EXIT_FAILURE);
  }

  TEXTURE_FILTERING_ALGORITHM algorithm =
      trilinear.Get() ? TEXTURE_FILTERING_ALGORITHM_TRILINEAR
                      : TEXTURE_FILTERING_ALGORITHM_EWA;

  ReflectorMipmap mipmap;
  ISTATUS status = PngReflectorMipmapAllocate(
      filename.Get().second.c_str(), algorithm, *maxanisotropy.Get(), wrap_mode,
      spectrum_manager.GetColorExtrapolator().get(),
      mipmap.release_and_get_address());
  switch (status) {
    case ISTATUS_IO_ERROR:
      std::cerr << "ERROR: Failed to read PNG file: " << filename.Get().first
                << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_ALLOCATION_FAILED:
      ReportOOM();
    default:
      assert(status == ISTATUS_SUCCESS);
  }

  return texture_manager.AllocateImageMapReflectorTexture(
      std::move(mipmap), *u_delta.Get(), *v_delta.Get(), *u_scale.Get(),
      *v_scale.Get());
}

FloatTexture ParseImageMapFloat(
    Parameters& parameters, const Matrix& texture_to_world,
    const NamedTextureManager& named_texture_manager,
    TextureManager& texture_manager) {
  SingleBoolMatcher trilinear("trilinear", false, kImageMapDefaultTrilinear);
  SingleFloatMatcher maxanisotropy("maxanisotropy", false, false, (float_t)0.0,
                                   std::numeric_limits<float_t>::infinity(),
                                   kImageMapDefaultMaxAnisotropy);
  SingleFloatMatcher u_scale(
      "uscale", false, false, -std::numeric_limits<float_t>::infinity(),
      std::numeric_limits<float_t>::infinity(), kImageMapDefaultUVScaleValue);
  SingleFloatMatcher v_scale(
      "vscale", false, false, -std::numeric_limits<float_t>::infinity(),
      std::numeric_limits<float_t>::infinity(), kImageMapDefaultUVScaleValue);
  SingleFloatMatcher u_delta(
      "udelta", false, false, -std::numeric_limits<float_t>::infinity(),
      std::numeric_limits<float_t>::infinity(), kImageMapDefaultUVDeltaValue);
  SingleFloatMatcher v_delta(
      "vdelta", false, false, -std::numeric_limits<float_t>::infinity(),
      std::numeric_limits<float_t>::infinity(), kImageMapDefaultUVDeltaValue);
  SingleFileMatcher filename("filename");
  SingleStringMatcher wrap("wrap", false, kImageMapDefaultWrap);
  parameters.Match(trilinear, maxanisotropy, u_scale, v_scale, u_delta, v_delta,
                   filename, wrap);

  if (!absl::EndsWith(filename.Get().first, ".png")) {
    std::cerr << "ERROR: png is the only supported image format" << std::endl;
    exit(EXIT_FAILURE);
  }

  WRAP_MODE wrap_mode;
  if (wrap.Get() == "repeat") {
    wrap_mode = WRAP_MODE_REPEAT;
  } else if (wrap.Get() == "black") {
    wrap_mode = WRAP_MODE_BLACK;
  } else if (wrap.Get() == "clamp") {
    wrap_mode = WRAP_MODE_CLAMP;
  } else {
    std::cerr << "ERROR: Unsupported wrap mode for ImageMap: " << wrap.Get()
              << std::endl;
    exit(EXIT_FAILURE);
  }

  TEXTURE_FILTERING_ALGORITHM algorithm =
      trilinear.Get() ? TEXTURE_FILTERING_ALGORITHM_TRILINEAR
                      : TEXTURE_FILTERING_ALGORITHM_EWA;

  FloatMipmap mipmap;
  ISTATUS status = PngFloatMipmapAllocate(
      filename.Get().second.c_str(), algorithm, *maxanisotropy.Get(), wrap_mode,
      mipmap.release_and_get_address());
  switch (status) {
    case ISTATUS_IO_ERROR:
      std::cerr << "ERROR: Failed to read PNG file: " << filename.Get().first
                << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_ALLOCATION_FAILED:
      ReportOOM();
    default:
      assert(status == ISTATUS_SUCCESS);
  }

  return texture_manager.AllocateImageMapFloatTexture(
      std::move(mipmap), *u_delta.Get(), *v_delta.Get(), *u_scale.Get(),
      *v_scale.Get());
}

}  // namespace iris