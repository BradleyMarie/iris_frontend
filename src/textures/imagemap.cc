#include "src/textures/imagemap.h"

#include <limits>

#include "absl/strings/match.h"
#include "iris_physx_toolkit/png_mipmap.h"
#include "src/param_matchers/float_single.h"
#include "src/param_matchers/float_texture.h"
#include "src/param_matchers/matcher.h"
#include "src/param_matchers/reflector_texture.h"
#include "src/param_matchers/single.h"

namespace iris {
namespace {

static const float_t kImageMapDefaultUVScaleValue = (float_t)1.0;
static const float_t kImageMapDefaultUVDeltaValue = (float_t)0.0;
static const char* kImageMapDefaultFilename = "";
static const char* kImageMapDefaultWrap = "repeat";

}  // namespace

ReflectorTexture ParseImageMapReflector(
    const char* base_type_name, const char* type_name, Tokenizer& tokenizer,
    const NamedTextureManager& named_texture_manager,
    TextureManager& texture_manager, SpectrumManager& spectrum_manager) {
  SingleFloatMatcher u_scale(base_type_name, type_name, "uscale", false, false,
                             -std::numeric_limits<float_t>::infinity(),
                             std::numeric_limits<float_t>::infinity(),
                             kImageMapDefaultUVScaleValue);
  SingleFloatMatcher v_scale(base_type_name, type_name, "vscale", false, false,
                             -std::numeric_limits<float_t>::infinity(),
                             std::numeric_limits<float_t>::infinity(),
                             kImageMapDefaultUVScaleValue);
  SingleFloatMatcher u_delta(base_type_name, type_name, "udelta", false, false,
                             -std::numeric_limits<float_t>::infinity(),
                             std::numeric_limits<float_t>::infinity(),
                             kImageMapDefaultUVDeltaValue);
  SingleFloatMatcher v_delta(base_type_name, type_name, "vdelta", false, false,
                             -std::numeric_limits<float_t>::infinity(),
                             std::numeric_limits<float_t>::infinity(),
                             kImageMapDefaultUVDeltaValue);
  SingleStringMatcher filename(base_type_name, type_name, "filename", true,
                               kImageMapDefaultFilename);
  SingleStringMatcher wrap(base_type_name, type_name, "wrap", true,
                           kImageMapDefaultWrap);
  MatchParameters<6>(
      base_type_name, type_name, tokenizer,
      {&u_scale, &v_scale, &u_delta, &v_delta, &filename, &wrap});

  if (!absl::EndsWith(filename.Get(), ".png")) {
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

  std::string resolved_path = tokenizer.ResolvePath(filename.Get());

  ReflectorMipmap mipmap;
  ISTATUS status =
      PngReflectorMipmapAllocate(resolved_path.c_str(), wrap_mode,
                                 spectrum_manager.GetColorExtrapolator().get(),
                                 mipmap.release_and_get_address());
  switch (status) {
    case ISTATUS_IO_ERROR:
      std::cerr << "ERROR: Failed to read PNG file: " << filename.Get()
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
    const char* base_type_name, const char* type_name, Tokenizer& tokenizer,
    const NamedTextureManager& named_texture_manager,
    TextureManager& texture_manager) {
  SingleFloatMatcher u_scale(base_type_name, type_name, "uscale", false, false,
                             -std::numeric_limits<float_t>::infinity(),
                             std::numeric_limits<float_t>::infinity(),
                             kImageMapDefaultUVScaleValue);
  SingleFloatMatcher v_scale(base_type_name, type_name, "vscale", false, false,
                             -std::numeric_limits<float_t>::infinity(),
                             std::numeric_limits<float_t>::infinity(),
                             kImageMapDefaultUVScaleValue);
  SingleFloatMatcher u_delta(base_type_name, type_name, "udelta", false, false,
                             -std::numeric_limits<float_t>::infinity(),
                             std::numeric_limits<float_t>::infinity(),
                             kImageMapDefaultUVDeltaValue);
  SingleFloatMatcher v_delta(base_type_name, type_name, "vdelta", false, false,
                             -std::numeric_limits<float_t>::infinity(),
                             std::numeric_limits<float_t>::infinity(),
                             kImageMapDefaultUVDeltaValue);
  SingleStringMatcher filename(base_type_name, type_name, "filename", true,
                               kImageMapDefaultFilename);
  SingleStringMatcher wrap(base_type_name, type_name, "wrap", true,
                           kImageMapDefaultWrap);
  MatchParameters<6>(
      base_type_name, type_name, tokenizer,
      {&u_scale, &v_scale, &u_delta, &v_delta, &filename, &wrap});

  if (!absl::EndsWith(filename.Get(), ".png")) {
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

  std::string resolved_path = tokenizer.ResolvePath(filename.Get());

  FloatMipmap mipmap;
  ISTATUS status = PngFloatMipmapAllocate(resolved_path.c_str(), wrap_mode,
                                          mipmap.release_and_get_address());
  switch (status) {
    case ISTATUS_IO_ERROR:
      std::cerr << "ERROR: Failed to read PNG file: " << filename.Get()
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