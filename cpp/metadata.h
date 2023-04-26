#include <jsi/jsi.h>

#include <fstream>
#include <string>

namespace jsi = facebook::jsi;

/**
 * A deleter for std::ifstream that closes the file before deleting it.
 */
struct FileStreamDeleter {
  void operator()(std::ifstream* file) const {
    file->close();
    delete file;
  }
};

bool isFilePNG(const std::string& path);

double getFileSize(const std::string& path);

jsi::Object getPNGMetadata(jsi::Runtime& rt, const std::string& path);
