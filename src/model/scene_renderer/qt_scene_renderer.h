#ifndef QT_SCENE_RENDERER_H_
#define QT_SCENE_RENDERER_H_

#define GL_SILENCE_DEPRECATION

#include <QColor>
#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>

#include "scene_renderer/scene_renderer_base.h"

class QtSceneRenderer : public QOpenGLWidget,
                        protected QOpenGLFunctions,
                        public SceneRendererBase {
  Q_OBJECT

 public:
  QtSceneRenderer(QWidget *parent = nullptr);
  ~QtSceneRenderer();

 private:
  /* Template functions */
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;
  /* Helpers */
  void DrawScene(Scene *scene) override {scene_ = scene;}
  void drawFigure(Figure figure);
  void drawAxes();

  /* Events */
  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;

 private:
  float view_default_scale_ = 0.7;
  float ar_;  // aspect ratio
  /* Camera control */
  QPoint mouse_pos_;
  float cam_speed_ = 0.2;
  float cam_rotx_ = 30, cam_roty_ = 30, cam_rotz_;
};

#endif  // QT_SCENE_RENDERER_H_
