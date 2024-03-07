#ifndef FACADE_H_
#define FACADE_H_

#include "file_reader/file_reader_base.h"
#include "scene.h"
#include "scene_renderer/scene_renderer_base.h"

class Facade {
 public:
  void LoadScene() { ; }
  void RenderScene() { ; }
  void SetSceneParams() { ; }
  void SetRendererParams() { ; }

 private:
  FileReaderBase file_reader_;
  SceneRendererBase scene_renderer_;
  Scene scene_;
};

#endif  // FACADE_H_
