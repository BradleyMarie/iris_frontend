#include "src/films/image.h"

#include "src/films/output_writers/parser.h"
#include "src/param_matcher.h"

#include <iostream>

namespace iris {
namespace {

static const size_t kImageFilmDefaultXResolution = 640;
static const size_t kImageFilmDefaultYResolution = 480;

}  // namespace

FilmResult ParseImage(const char* base_type_name, const char* type_name,
                      Tokenizer& tokenizer) {
  SingleStringMatcher filename(base_type_name, type_name, "filename",
                               GetDefaultOutputFilename());
  NonZeroSingleSizeTMatcher xresolution(
      base_type_name, type_name, "xresolution", kImageFilmDefaultXResolution);
  NonZeroSingleSizeTMatcher yresolution(
      base_type_name, type_name, "yresolution", kImageFilmDefaultYResolution);
  ParseAllParameter<3>(base_type_name, type_name, tokenizer,
                       {&filename, &xresolution, &yresolution});

  Framebuffer framebuffer;
  ISTATUS status = FramebufferAllocate(xresolution.Get(), yresolution.Get(),
                                       framebuffer.release_and_get_address());
  switch (status) {
    case ISTATUS_ALLOCATION_FAILED:
      std::cerr << "ERROR: Allocation failed" << std::endl;
      exit(EXIT_FAILURE);
    default:
      assert(status == ISTATUS_SUCCESS);
  }

  return std::make_pair(std::move(framebuffer),
                        ParseOutputWriter(filename.Get()));
}

}  // namespace iris