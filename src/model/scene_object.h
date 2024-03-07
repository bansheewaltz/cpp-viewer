#ifndef SCENE_OBJECT_H_
#define SCENE_OBJECT_H_

#include "transform_matrix.h"

class SceneObject {
 public:
  ~SceneObject() {}
  virtual void Transform(const TransformMatrix &m) = 0;
};

#endif  // SCENE_OBJECT_H_
