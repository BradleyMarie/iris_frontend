#ifndef _SRC_FILMS_OUTPUT_WRITER_LOCK_FILE_
#define _SRC_FILMS_OUTPUT_WRITER_LOCK_FILE_

#include <memory>
#include <string>

namespace iris {

class LockFile {
 public:
  static std::unique_ptr<LockFile> Create(const std::string& path);
  ~LockFile();

 private:
  LockFile(const std::string& path);

  std::string m_path;
};

}  // namespace iris

#endif  // _SRC_FILMS_OUTPUT_WRITER_LOCK_FILE_