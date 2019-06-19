#include "src/films/output_writers/pfm.h"

#include <iostream>
#include <memory>

#include "iris_camera_toolkit/pfm_writer.h"

namespace iris {

OutputWriter ParsePfm(absl::string_view file_name) {
  std::string path(file_name);
  FILE* lock_file = fopen(path.c_str(), "w+");
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

    ISTATUS status =
        WriteToPfmFile(framebuffer.get(), path.c_str(), PFM_PIXEL_FORMAT_SRGB);
    switch (status) {
      case ISTATUS_IO_ERROR:
        std::cerr << "ERROR: Failed to write to output file: " << path
                  << std::endl;
        exit(EXIT_FAILURE);
      default:
        assert(status == ISTATUS_SUCCESS);
    }
  };
}

}  // namespace iris