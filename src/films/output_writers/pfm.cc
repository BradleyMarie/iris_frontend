#include "src/films/output_writers/pfm.h"

#include <iostream>
#include <memory>

#include "iris_camera_toolkit/pfm_writer.h"
#include "src/films/output_writers/lock_file.h"

namespace iris {
namespace {

class PfmWriter final : public OutputWriterBase {
 public:
  static std::unique_ptr<OutputWriterBase> Create(absl::string_view file_name);
  void Write(const Framebuffer& framebuffer);

 private:
  PfmWriter(std::string path);

  std::unique_ptr<LockFile> m_lock_file;
  std::string m_path;
};

std::unique_ptr<OutputWriterBase> PfmWriter::Create(
    absl::string_view file_name) {
  std::string path(file_name);
  return std::unique_ptr<OutputWriterBase>(new PfmWriter(std::move(path)));
}

PfmWriter::PfmWriter(std::string path)
    : m_lock_file(LockFile::Create(path)), m_path(std::move(path)) {}

void PfmWriter::Write(const Framebuffer& framebuffer) {
  m_lock_file.reset();

  ISTATUS status =
      WriteToPfmFile(framebuffer.get(), m_path.c_str(), PFM_PIXEL_FORMAT_SRGB);
  switch (status) {
    case ISTATUS_IO_ERROR:
      std::cerr << "ERROR: Failed to write to output file: " << m_path
                << std::endl;
      exit(EXIT_FAILURE);
    default:
      assert(status == ISTATUS_SUCCESS);
  }
}

}  // namespace

OutputWriter ParsePfm(absl::string_view file_name) {
  return PfmWriter::Create(file_name);
}

}  // namespace iris