#ifndef FIGURE_H_
#define FIGURE_H_

#include <vector>

#include "primitives.h"
#include "scene_object.h"
#include "transform_matrix.h"

class Figure : SceneObject {
 public:
  std::vector<Edge> edges() { return edges_; }
  bool is_axes_enabled() { return is_axes_enabled_; }
  void Transform(const TransformMatrix& m) override { ; }

 private:
  std::vector<Edge> edges_;
  bool is_axes_enabled_ = true;
};

#endif  // FIGURE_H_
