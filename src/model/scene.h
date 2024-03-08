#ifndef SCENE_H_
#define SCENE_H_

#include "dtos.h"
#include "figure.h"
#include "transform_matrix.h"
#include "transform_matrix_builder.h"

class Scene {
 public:
  Figure figure() const { return figures_[0]; }
  std::vector<Figure> figures() const { return figures_; }
  TransformMatrix transform_matrix() const { return transform_matrix_; }
  TransformMatrix normalization_matrix() const { return normalization_matrix_; }
  FigureStatsDTO stats() { return figures_[0].stats(); }
  void set_normalization_matrix(TransformMatrix norm) {
    normalization_matrix_ = norm;
  }

  void AddFigure(Figure figure) { figures_.push_back(figure); }
  void ReplaceFigure(Figure figure) { figures_[figures_.size() - 1] = figure; }
  void TransformScene(const SceneTransformsDTO transforms) {
    auto &t = transforms;
    auto translation_m = TransformMatrixBuilder::CreateTranslationMatrix(
        t.trnsx, t.trnsy, t.trnsz);
    auto rotation_m = TransformMatrixBuilder::CreateRotationMatrix(  //
        t.rotx, t.roty, t.rotz);
    auto scale_m = TransformMatrixBuilder::CreateScaleMatrix(
        t.scalex, t.scaley, t.scalez, t.scaleu);
    transform_matrix_ = translation_m * rotation_m * scale_m;
  }

 private:
  std::vector<Figure> figures_;
  TransformMatrix transform_matrix_;
  TransformMatrix normalization_matrix_;
};

#endif  // SCENE_H_
