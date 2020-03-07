#include "src/films/output_writers/exr.h"

#include <iostream>
#include <memory>
#include <vector>

#include "src/films/output_writers/lock_file.h"
#include "third_party/tinyexr/tinyexr.h"

namespace iris {
namespace {

class ExrWriter final : public OutputWriterBase {
 public:
  static std::unique_ptr<OutputWriterBase> Create(absl::string_view file_name);
  void Write(const Framebuffer& framebuffer);

 private:
  ExrWriter(std::string path);

  std::unique_ptr<LockFile> m_lock_file;
  std::string m_path;
};

std::unique_ptr<OutputWriterBase> ExrWriter::Create(
    absl::string_view file_name) {
  std::string path(file_name);
  return std::unique_ptr<OutputWriterBase>(new ExrWriter(std::move(path)));
}

ExrWriter::ExrWriter(std::string path)
    : m_lock_file(LockFile::Create(path)), m_path(std::move(path)) {}

void ExrWriter::Write(const Framebuffer& framebuffer) {
  m_lock_file.reset();

  size_t columns, rows;
  FramebufferGetSize(framebuffer.get(), &columns, &rows);

  std::vector<float> data;
  for (size_t row = 0; row < rows; row++) {
    for (size_t column = 0; column < columns; column++) {
      COLOR3 pixel;
      FramebufferGetPixel(framebuffer.get(), column, row, &pixel);

      pixel = ColorConvert(pixel, COLOR_SPACE_LINEAR_SRGB);
      data.push_back(pixel.values[0]);
      data.push_back(pixel.values[1]);
      data.push_back(pixel.values[2]);
    }
  }

  int status = TINYEXR_SUCCESS;
  if (INT_MAX < columns || INT_MAX < rows) {
    status = TINYEXR_ERROR_INVALID_DATA;
  } else {
    status = SaveEXR(data.data(), static_cast<int>(columns),
                     static_cast<int>(rows), 3, 1, m_path.c_str(), nullptr);
  }

  if (status != TINYEXR_SUCCESS) {
    std::cerr << "ERROR: Failed to write to output file: " << m_path
              << std::endl;
    exit(EXIT_FAILURE);
  }
}

}  // namespace

OutputWriter ParseExr(absl::string_view file_name) {
  return ExrWriter::Create(file_name);
}

}  // namespace iris