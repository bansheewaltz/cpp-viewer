#ifndef DTOS_H_
#define DTOS_H_

#include "color.h"

struct RendererParamsDTO {
  enum class ProjectionType { ORTHOGONAL, PERSPECTIVE };
  ProjectionType projection_type = ProjectionType::ORTHOGONAL;

  Color background_color{0xEFE5D7};

  bool is_show_axes = true;

  bool is_show_lines = true;
  enum class LineStyle { SOLID, DASHED };
  LineStyle line_style = LineStyle::SOLID;
  Color line_color{0x974F4C};
  float line_width = 2.0f;

  bool is_show_points = true;
  enum class PointStyle { CIRCLE, SQUARE };
  PointStyle point_style = PointStyle::CIRCLE;
  Color point_color = line_color;
  float point_size = 13.0f;
};

struct SceneTransformsDTO {
  float rotx, roty, rotz;
  float trnsx, trnsy, trnsz;
  float scalex = 1, scaley = 1, scalez = 1, scaleu = 1;
};

struct FigureStatsDTO {
  std::string filepath;
  int vertices_cnt;
  int edges_cnt, edges_unique_cnt;
  int faces_cnt;
};

#endif  // DTOS_H_
