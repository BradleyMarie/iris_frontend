#include <cstdio>
#include <iostream>
#include <memory>

#include "absl/strings/match.h"
#include "iris_camera_toolkit/pfm_writer.h"
#include "src/output_writer.h"

namespace iris {

OutputWriter ParseOutputWriter(absl::string_view path) {
  if (!absl::EndsWith(path, ".pfm")) {
    std::cerr << "ERROR: pfm is the only supported output format" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::string file_name(path);
  FILE* lock_file = fopen(file_name.c_str(), "w+");
  if (!lock_file) {
    std::cerr << "ERROR: Failed to open output file: " << path << std::endl;
    exit(EXIT_FAILURE);
  }

  auto shared_lock_file = std::make_shared<FILE*>(lock_file);

  return [=](const Framebuffer& framebuffer) {
    if (*shared_lock_file) {
      if (fclose(*shared_lock_file) != 0) {
        std::cerr << "ERROR: Failed to release lock on output file"
                  << std::endl;
        exit(EXIT_FAILURE);
      }

      *shared_lock_file = nullptr;
    }

    ISTATUS status = WriteToPfmFile(framebuffer.get(), file_name.c_str(),
                                    PFM_PIXEL_FORMAT_SRGB);
    switch (status) {
      case ISTATUS_IO_ERROR:
        std::cerr << "ERROR: Failed to write to output file: " << file_name
                  << std::endl;
        exit(EXIT_FAILURE);
      default:
        assert(status == ISTATUS_SUCCESS);
    }
  };
}

}  // namespace iris