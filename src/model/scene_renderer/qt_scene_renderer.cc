#include "qt_scene_renderer.h"

#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QWidget>

#include "opengl_helpers.h"
#include "transform_matrix_builder.h"

QtSceneRenderer::QtSceneRenderer(QWidget *parent) : QOpenGLWidget(parent) {
  scene_ = new Scene;
  scene_->AddFigure(Figure::CreateCube(0, 0, 0, 0.5));
}
QtSceneRenderer::~QtSceneRenderer() {}

/* Set up the rendering context, load shaders and other resources, etc. */
void QtSceneRenderer::initializeGL() {
  /* Retrieve OpenGL functions from graphics card's drivers */
  initializeOpenGLFunctions();
  /* Set proper rendering of overlapping elements */
  glEnable(GL_DEPTH_TEST);
  glEnableClientState(GL_VERTEX_ARRAY);
}

/* Update the viewport size */
void QtSceneRenderer::resizeGL(int w, int h) {
  glViewport(0, 0, w, h);
  ar_ = float(w) / h;
}

/* Draw the scene */
void QtSceneRenderer::paintGL() {
  /* Set the background color */
  auto bc = params_.background_color;
  glClearColor(bc.red_f(), bc.green_f(), bc.blue_f(), bc.alpha_f());
  /* Clear the buffers */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* Set the projection matrix */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (params_.projection_type ==
      RendererParamsDTO::ProjectionType::PERSPECTIVE) {
    glFrustum(-0.5 * ar_, 0.5 * ar_, -0.5, 0.5, 1, 1000);
    glTranslatef(0, 0, -2 * ar_);
  } else {
    static const float c = sqrt(3);  // because of the view cube with side 2
    glOrtho(-c * ar_, c * ar_, -c, c, -10, 10);
  }

  /* Set the model-view matrix */
  glMatrixMode(GL_MODELVIEW);
  /* Perform all the transformations */
  auto view_rotate =
      TransformMatrixBuilder::CreateRotationMatrix(cam_rotx_, cam_roty_);
  if (params_.is_show_axes) {
    glLoadMatrixf(view_rotate.data());
    drawAxes();
  }
  auto transform = view_rotate * scene_->transform_matrix();
  if (scene_->figure().is_show_axes()) {
    glLoadMatrixf(transform.data());
    drawAxes();
  }
  transform = transform * scene_->normalization_matrix();
  glLoadMatrixf(transform.data());

  drawFigure(scene_->figure());
}

void QtSceneRenderer::drawFigure(Figure figure) {
  /* Set up the buffers */
  glVertexPointer(3, GL_FLOAT, 0, figure.vertices().data());
  /* Draw lines */
  if (params_.is_show_lines) {
    Color lc = params_.line_color;
    glColor3d(lc.red_f(), lc.green_f(), lc.blue_f());
    glLineWidth(params_.line_width);
    if (params_.line_style == RendererParamsDTO::LineStyle::DASHED) {
      glEnable(GL_LINE_STIPPLE);
      glLineStipple(10, 0x3333);
    }
    GLCall(glDrawElements(GL_LINES, figure.line_index_array().size(),
                          GL_UNSIGNED_INT, figure.line_index_array().data()));
    glDisable(GL_LINE_STIPPLE);
  }
  /* Draw points */
  if (params_.is_show_points) {
    Color pc = params_.point_color;
    glColor3d(pc.red_f(), pc.green_f(), pc.blue_f());
    glPointSize(params_.point_size);
    if (params_.point_style == RendererParamsDTO::PointStyle::CIRCLE) {
      glEnable(GL_POINT_SMOOTH);
    }
    GLCall(glDrawArrays(GL_POINTS, 1, (figure.vertices().size() - 1) / 3));
    glDisable(GL_POINT_SMOOTH);
  }
}

void QtSceneRenderer::drawAxes() {
  static float fmax = 1000;
  static float fmin = -fmax;

  static unsigned int position_count = 6;
  static float positions[] = {
      fmin, 0,    0,     // -x
      fmax, 0,    0,     // +x
      0,    fmin, 0,     // -y
      0,    fmax, 0,     // +y
      0,    0,    fmin,  // -z
      0,    0,    fmax,  // +z
  };

  glVertexPointer(3, GL_FLOAT, 0, positions);
  glLineWidth(1);
  QColor lc = "#9A9A9A";
  glColor3d(lc.redF(), lc.greenF(), lc.blueF());
  glDrawArrays(GL_LINES, 0, position_count);
}

/* Interactive rotation in the viewport with a mouse */
void QtSceneRenderer::mousePressEvent(QMouseEvent *m) { mouse_pos_ = m->pos(); }
void QtSceneRenderer::mouseMoveEvent(QMouseEvent *m) {
  cam_rotx_ += cam_speed_ * (m->pos().y() - mouse_pos_.y());
  cam_roty_ += cam_speed_ * (m->pos().x() - mouse_pos_.x());
  mouse_pos_ = m->pos();
  update();
}
