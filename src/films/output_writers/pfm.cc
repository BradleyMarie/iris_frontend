#include "src/films/output_writers/pfm.h"

#include <iostream>
#include <memory>

#include "iris_camera_toolkit/pfm_writer.h"

namespace iris {

class PfmWriter final : public OutputWriterBase {
 public:
  static OutputWriter Create(absl::string_view file_name);
  void Write(const Framebuffer& framebuffer);

 private:
  PfmWriter(std::string path, FILE* lock_file);
  void ReleaseLock(bool report_errors);
  ~PfmWriter();

  std::string m_path;
  FILE* m_lock_file;
};

PfmWriter::PfmWriter(std::string path, FILE* lock_file)
    : m_path(path), m_lock_file(lock_file) {}

OutputWriter PfmWriter::Create(absl::string_view path) {
  std::string path_copy(path);
  FILE* lock_file = fopen(path_copy.c_str(), "w+");
  if (!lock_file) {
    std::cerr << "ERROR: Failed to open output file: " << path << std::endl;
    exit(EXIT_FAILURE);
  }

  return std::unique_ptr<OutputWriterBase>(
      new PfmWriter(std::move(path_copy), lock_file));
}

void PfmWriter::Write(const Framebuffer& framebuffer) {
  ReleaseLock(true);

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

void PfmWriter::ReleaseLock(bool report_errors) {
  assert(m_lock_file != NULL);

  if (fclose(m_lock_file) != 0 && report_errors) {
    std::cerr << "ERROR: Failed to release lock on output file" << std::endl;
    exit(EXIT_FAILURE);
  }

  m_lock_file = NULL;
}

PfmWriter::~PfmWriter() {
  if (m_lock_file) {
    ReleaseLock(false);
  }
}

OutputWriter ParsePfm(absl::string_view file_name) {
  return PfmWriter::Create(file_name);
}

}  // namespace iris