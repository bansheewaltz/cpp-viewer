#ifndef SCENE_H_
#define SCENE_H_

#include <vector>

#include "figure.h"
#include "transform_matrix.h"

class Scene {
 public:
  std::vector<Figure> figures() { return figures_; }
  bool is_axes_enabled() { return is_axes_enabled_; }
  TransformMatrix transform_matrix() { return transform_matrix_; }

 private:
  std::vector<Figure> figures_;
  bool is_axes_enabled_ = true;
  TransformMatrix transform_matrix_;
};

#endif  // SCENE_H_
