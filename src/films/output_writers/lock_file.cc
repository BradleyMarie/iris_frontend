#include "src/films/output_writers/lock_file.h"

#include <cassert>
#include <cstdio>
#include <iostream>
#include <map>
#include <mutex>

namespace iris {
namespace {

static std::once_flag g_cleanup_callback_registered;
static std::map<std::string, std::pair<FILE*, bool>> g_lock_files;

static void Release(const std::string& path, bool is_exiting) {
  auto& entry = g_lock_files.at(path);

  if (fclose(entry.first) != 0 && !is_exiting) {
    std::cerr << "ERROR: Failed to release lock on output file" << std::endl;
    exit(EXIT_FAILURE);
  }

  if (!is_exiting) {
    entry.first = NULL;
  }

  if (!entry.second && remove(path.c_str()) != 0 && !is_exiting) {
    std::cerr << "ERROR: Failed to release lock on output file" << std::endl;
    exit(EXIT_FAILURE);
  }

  if (!is_exiting) {
    g_lock_files.erase(path);
  }
}

static void CleanupOnExit() {
  for (const auto& entry : g_lock_files) {
    Release(entry.first, true);
  }
}

}  // namespace

LockFile::LockFile(const std::string& path) : m_path(path) {}

std::unique_ptr<LockFile> LockFile::Create(const std::string& path) {
  std::call_once(g_cleanup_callback_registered, []() {
    if (atexit(CleanupOnExit) != 0) {
      std::cerr << "ERROR: Failed to register lock file exit handler"
                << std::endl;
      exit(EXIT_FAILURE);
    }
  });

  bool file_already_exists;
  FILE* lock_file = fopen(path.c_str(), "wx");
  if (lock_file) {
    file_already_exists = false;
  } else {
    lock_file = fopen(path.c_str(), "a");
    if (!lock_file) {
      std::cerr << "ERROR: Failed to open output file: " << path << std::endl;
      exit(EXIT_FAILURE);
    }

    file_already_exists = true;
  }

  assert(g_lock_files.count(path) == 0);
  g_lock_files.emplace(path, std::make_pair(lock_file, file_already_exists));

  return std::unique_ptr<LockFile>(new LockFile(path));
}

LockFile::~LockFile() { Release(m_path, false); }

}  // namespace iris