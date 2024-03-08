#ifndef SCENE_RENDERER_BASE_H_
#define SCENE_RENDERER_BASE_H_

#include "dtos.h"
#include "scene.h"

class SceneRendererBase {
 public:
  void set_params(RendererParamsDTO params) { params_ = params; }
  virtual void DrawScene(Scene* scene) = 0;

 protected:
  RendererParamsDTO params_;
  Scene* scene_;
};

#endif  // SCENE_RENDERER_BASE_H_
