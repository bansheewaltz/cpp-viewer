#ifndef FILE_READER_BASE_H_
#define FILE_READER_BASE_H_

#include "scene.h"

class FileReaderBase {
 public:
  virtual Scene ReadScene(std::string path) = 0;
};

#endif  // FILE_READER_BASE_H_
