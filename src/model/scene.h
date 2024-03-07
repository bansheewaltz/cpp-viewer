#ifndef SCENE_H_
#define SCENE_H_

#include <vector>

#include "figure.h"
#include "params_dto.h"
#include "transform_matrix.h"

class Scene {
 public:
  std::vector<Figure> figures() { return figures_; }
  bool is_show_axes() { return is_show_axes_; }
  TransformMatrix transform_matrix() { return transform_matrix_; }
  void set_params(const SceneParamsDTO params) { params_ = params; }
  FigureStatsDTO get_stats() { return figures_[0].get_stats(); }

 private:
  std::vector<Figure> figures_;
  bool is_show_axes_ = true;
  SceneParamsDTO params_;
  TransformMatrix transform_matrix_;
};

#endif  // SCENE_H_
