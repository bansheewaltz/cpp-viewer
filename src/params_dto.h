#ifndef DTOS_H_
#define DTOS_H_

#include "color.h"

struct RendererParamsDTO {
  float aspect_ratio;
  Color background_color{"#EFE5D7"};

  bool is_show_lines_enabled;
  enum class LineStyle { SOLID, DASHED };
  LineStyle line_style;
  Color line_color{"#974F4C"};
  float line_width;

  bool is_show_points_enabled;
  enum class PointStyle { CIRCLE, SQUARE };
  PointStyle point_style;
  Color point_color = line_color;
  float point_size;
};

struct SceneParamsDTO {
  enum class ProjectionType { ORTHOGONAL, PERSPECTIVE };
  ProjectionType projection_type;

  float rotx, roty, rotz;
  float trnsx, trnsy, trnsz;
  float scalex, scaley, scalez, scaleu;
};

struct FigureStatsDTO {
  std::string filename;
  int vertices_cnt;
  int edges_cnt, edges_unique_cnt;
  int faces_cnt;
};

#endif  // DTOS_H_
