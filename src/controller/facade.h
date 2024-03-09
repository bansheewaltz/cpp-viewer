#ifndef FACADE_H_
#define FACADE_H_

#include "dtos.h"
#include "file_reader/file_reader_base.h"
#include "scene.h"
#include "scene_renderer/scene_renderer_base.h"

class Facade {
 public:
  void LoadScene(std::string filepath) {
    scene_ = file_reader_->ReadScene(filepath);
  }
  void CreateDefaultScene() {
    scene_ = Scene::CreateDefaultScene();
    scene_renderer_->DrawScene(&scene_);
  }

  void DrawScene() { scene_renderer_->DrawScene(&scene_); }
  void TransformScene(SceneTransformsDTO transforms) {
    scene_.TransformScene(transforms);
  }
  void SetRendererParams(RendererParamsDTO params) {
    scene_renderer_->set_params(params);
  }
  void SetSceneRenderer(SceneRendererBase* scene_renderer) {
    scene_renderer_ = scene_renderer;
  }
  void SetFileReader(FileReaderBase* file_reader) {
    file_reader_ = file_reader;
  }

  FigureStatsDTO scene_stats() { return scene_.stats(); }

 private:
  FileReaderBase* file_reader_;
  SceneRendererBase* scene_renderer_;
  Scene scene_;
};

#endif  // FACADE_H_
