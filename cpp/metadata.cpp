#include "metadata.h"

#include <jsi/jsi.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "png.h"

namespace jsi = facebook::jsi;

void pngReadCallback(png_structp pngPtr, png_bytep data, png_size_t length) {
  auto input = reinterpret_cast<std::istream*>(png_get_io_ptr(pngPtr));
  input->read(reinterpret_cast<char*>(data), length);
}

bool isFilePNG(const std::string& path) {
  std::unique_ptr<std::ifstream, FileStreamDeleter> fileStream(new std::ifstream(path, std::ios::in | std::ios::binary));
  if (!fileStream || !fileStream->is_open()) {
    return false;
  }

  // Read the first 8 bytes of the file
  std::vector<unsigned char> header(8);
  fileStream->read((char*)header.data(), header.size());

  // All pngs begin with the following 8 bytes: 0x89 0x50 0x4E 0x47 0x0D 0x0A 0x1A 0x0A
  const std::vector<unsigned char> pngMagic = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};

  // Check if the file matches the PNG magic number
  if (std::equal(header.begin(), header.end(), pngMagic.begin())) {
    return true;
  }

  return false;
}

double getFileSize(const std::string& path) {
  std::unique_ptr<std::ifstream, FileStreamDeleter> inputFile(new std::ifstream(path, std::ios::in | std::ios::binary));
  inputFile->seekg(0, std::ios::end);
  std::streamsize fileSize = inputFile->tellg();
  inputFile->seekg(0, std::ios::beg);

  return static_cast<double>(fileSize);
}

jsi::Object getPNGMetadata(jsi::Runtime& rt, const std::string& path) {
  if (!isFilePNG(path)) {
    throw jsi::JSError(rt, "The file is not a PNG.");
  }

  std::unique_ptr<std::ifstream, FileStreamDeleter> inputFile(new std::ifstream(path, std::ios::in | std::ios::binary));
  if (!inputFile || !inputFile->is_open()) {
    throw jsi::JSError(rt, "Failed to open input file at path: " + path);
  }

  // Initialize PNG structures
  png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
  if (!pngPtr) {
    throw jsi::JSError(rt, "Failed to create PNG read struct");
  }

  png_infop infoPtr = png_create_info_struct(pngPtr);
  if (!infoPtr) {
    png_destroy_read_struct(&pngPtr, nullptr, nullptr);
    throw jsi::JSError(rt, "Failed to create PNG info struct");
  }

  // Set error handling
  if (setjmp(png_jmpbuf(pngPtr))) {
    png_destroy_read_struct(&pngPtr, &infoPtr, nullptr);
    throw jsi::JSError(rt, "Error during PNG metadata retrieval");
  }

  // Initialize PNG IO
  png_set_read_fn(pngPtr, static_cast<void*>(inputFile.get()), pngReadCallback);

  // Read PNG info
  png_read_info(pngPtr, infoPtr);

  png_uint_32 width, height;
  int bitDepth, colorType, interlaceMethod, compressionMethod, filterMethod;
  png_get_IHDR(pngPtr, infoPtr, &width, &height, &bitDepth, &colorType, &interlaceMethod, &compressionMethod, &filterMethod);

  // Clean up
  png_destroy_read_struct(&pngPtr, &infoPtr, nullptr);

  // Get file size
  double size = getFileSize(path);

  // Create an object to dump all our metadata into and return to the original caller.
  jsi::Object result(rt);

  result.setProperty(rt, "width", static_cast<int>(width));
  result.setProperty(rt, "height", static_cast<int>(height));
  result.setProperty(rt, "size", size);
  result.setProperty(rt, "bitDepth", bitDepth);
  result.setProperty(rt, "colorType", colorType);
  result.setProperty(rt, "interlaceMethod", interlaceMethod);
  result.setProperty(rt, "compressionMethod", compressionMethod);
  result.setProperty(rt, "filterMethod", filterMethod);

  return result;
}