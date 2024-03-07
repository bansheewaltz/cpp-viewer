#ifndef SCENE_RENDERER_BASE_H_
#define SCENE_RENDERER_BASE_H_

#include "scene.h"

class SceneRendererBase {
 protected:
  virtual void DrawScene(Scene *scene) = 0;
};

#endif  // SCENE_RENDERER_BASE_H_
