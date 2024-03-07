#ifndef FILE_READER_BASE_H_
#define FILE_READER_BASE_H_

#include <string>
#include <vector>

#include "lowlevel_file_reader.h"
#include "primitives.h"
#include "scene.h"

class FileReaderBase {
 public:
  Scene ReadScene(std::string path) { ; }

 protected:
  std::vector<Edge> edges_;
};

#endif  // FILE_READER_BASE_H_
