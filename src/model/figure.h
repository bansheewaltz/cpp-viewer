#ifndef FIGURE_H_
#define FIGURE_H_

#include <vector>

#include "dtos.h"
#include "scene_object.h"
#include "transform_matrix.h"

class Figure : SceneObject {
  friend class LowlevelFileReader;

 public:
  bool is_show_axes() { return is_show_axes_; }
  std::vector<unsigned>& line_index_array() { return line_index_array_; }
  FigureStatsDTO stats() { return stats_; }
  std::vector<float>& vertices() { return vertices_; }

  void Transform(const TransformMatrix& m) override { ; }

  static Figure CreateCube(float x, float y, float z, float side_len) {
    const float hside = side_len / 2;
    static const unsigned int vertex_count = 8;
    std::vector<float> vertices = {
        x + hside, y - hside, z + hside,  // right bottom front
        x + hside, y - hside, z - hside,  // right bottom back
        x - hside, y - hside, z + hside,  // left bottom front
        x - hside, y - hside, z - hside,  // left bottom back

        x + hside, y + hside, z + hside,  // right top front
        x + hside, y + hside, z - hside,  // right top back
        x - hside, y + hside, z + hside,  // left top front
        x - hside, y + hside, z - hside,  // left top back
    };
    static const unsigned int face_count = 6;
    static const std::vector<unsigned> face_vertex_counts = {4, 4, 4, 4, 4, 4};
    static const unsigned int index_count = 24;
    static const std::vector<unsigned> indices = {3, 2, 0, 1,   // bottom
                                                  4, 5, 7, 6,   // top
                                                  2, 3, 7, 6,   // left
                                                  0, 1, 5, 4,   // right
                                                  0, 4, 6, 2,   // near
                                                  1, 3, 7, 5};  // far
    Figure figure;
    figure.vertices_ = vertices;
    figure.indices_ = indices;
    figure.face_vertex_counts_ = face_vertex_counts;
    figure.line_index_array_ = {
        0, 1, 1, 3, 3, 2, 2, 0,  // bottom
        4, 5, 5, 7, 7, 6, 6, 4,  // top
        0, 4, 1, 5, 2, 6, 3, 7   // side edges
    };
    figure.stats_.vertices_cnt = 8;
    figure.stats_.faces_cnt = 6;
    figure.stats_.edges_cnt = 12;
    figure.stats_.edges_unique_cnt = 12;
    figure.stats_.filepath = "internal example";

    return figure;
  };

 private:
  std::vector<unsigned> line_index_array_;
  TransformMatrix normalization_matrix_;
  bool is_show_axes_ = false;
  FigureStatsDTO stats_;
  // internal representation
  std::vector<float> vertices_;
  std::vector<unsigned> face_vertex_counts_;
  std::vector<unsigned> indices_;
};

#endif  // FIGURE_H_
