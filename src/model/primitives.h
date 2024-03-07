#ifndef PRIMITIVES_H_
#define PRIMITIVES_H_

#include "scene_object.h"
#include "transform_matrix.h"

struct _3DPoint {
  float x, y, z;
};

class Vertex : SceneObject {
 public:
  _3DPoint position() { return position_; }
  void Transform(const TransformMatrix &transform) override { ; }

 private:
  _3DPoint position_;
};

class Edge {
 public:
  Vertex begin() { return begin_; }
  Vertex end() { return end_; }

 private:
  Vertex begin_;
  Vertex end_;
};

#endif  // PRIMITIVES_H_
