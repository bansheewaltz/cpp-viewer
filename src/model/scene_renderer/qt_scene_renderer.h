#ifndef QT_SCENE_RENDERER_H_
#define QT_SCENE_RENDERER_H_

#define GL_SILENCE_DEPRECATION

#include <QColor>
#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>

#include "scene_renderer/scene_renderer_base.h"

class QtSceneRenderer : public SceneRendererBase,
                        public QOpenGLWidget,
                        protected QOpenGLFunctions {
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
  virtual void drawAxes();
  virtual void drawFigure(const Figure *figure);
  virtual void drawScene(const Scene *scene);
  virtual void drawCube(float, float, float, float);

  void drawCubeScene();
  /* Events */
  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;

 private:
  float viewport_default_scale = 0.7;
  /* Camera control */
  float camera_speed;
  QPoint mouse_pos;
  float camera_rotx, camera_roty, camera_rotz;
};

#endif  // QT_SCENE_RENDERER_H_
