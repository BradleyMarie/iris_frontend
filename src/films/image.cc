#include "src/films/image.h"

#include <iostream>

#include "src/common/error.h"
#include "src/films/output_writers/parser.h"
#include "src/param_matchers/integral_single.h"
#include "src/param_matchers/matcher.h"
#include "src/param_matchers/single.h"

namespace iris {
namespace {

static const size_t kImageFilmDefaultXResolution = 640;
static const size_t kImageFilmDefaultYResolution = 480;

}  // namespace

FilmResult ParseImage(const char* base_type_name, const char* type_name,
                      Tokenizer& tokenizer) {
  SingleStringMatcher filename(base_type_name, type_name, "filename", false,
                               GetDefaultOutputFilename());
  NonZeroSingleSizeTMatcher xresolution(base_type_name, type_name,
                                        "xresolution", false,
                                        kImageFilmDefaultXResolution);
  NonZeroSingleSizeTMatcher yresolution(base_type_name, type_name,
                                        "yresolution", false,
                                        kImageFilmDefaultYResolution);
  MatchParameters(base_type_name, type_name, tokenizer,
                  {&filename, &xresolution, &yresolution});

  Framebuffer framebuffer;
  ISTATUS status = FramebufferAllocate(xresolution.Get(), yresolution.Get(),
                                       framebuffer.release_and_get_address());
  SuccessOrOOM(status);

  return std::make_pair(std::move(framebuffer),
                        ParseOutputWriter(filename.Get()));
}

}  // namespace iris