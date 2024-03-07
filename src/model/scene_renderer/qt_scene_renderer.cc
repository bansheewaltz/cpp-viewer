#include "qt_scene_renderer.h"

#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QWidget>
#include <iostream>

#include "model/processing.h"

#define VIEWCUBE_SIDE 2.0f

OpenGLWidget::OpenGLWidget(QWidget *parent) : QOpenGLWidget(parent) {
  resetSettings();
}
OpenGLWidget::~OpenGLWidget() { owv_mesh_destroy((OWV_Mesh *)this->mesh); }

void OpenGLWidget::resetSettings() {
  this->mesh = nullptr;
  this->index_array = nullptr;
  /* Colors */
  setBackgroundColor(background_color_default);
  setLineColor(line_color_default);
  setPointColor(point_color_default);
  /* Camera */
  setCameraSpeed(0.2f);
  setCameraRotationX(+30);
  setCameraRotationY(+40);
  setCameraRotationZ(0);
}

static void GLClearError() {
  while (glGetError() != GL_NO_ERROR)
    ;
}
static void GLCheckError() {
  while (GLenum error = glGetError()) {
    std::cout << "OpenGL error: " << error << std::endl;
  }
}

void printMatrix(float *m, bool native) {
  if (native)
    qDebug() << "native";
  else
    qDebug() << "custom";

  qDebug() << "address:" << &(m[0]);
  for (int i = 0; i < 16; i += 4)
    qDebug() << m[i] << m[i + 1] << m[i + 2] << m[i + 3];

  qDebug() << "";
}

/* Set up the rendering context, load shaders and other resources, etc. */
void OpenGLWidget::initializeGL() {
  /* Retrieve OpenGL functions from graphics card's drivers */
  initializeOpenGLFunctions();
  /* Set proper rendering of overlapping elements */
  glEnable(GL_DEPTH_TEST);
}

/* Update the viewport size */
void OpenGLWidget::resizeGL(int w, int h) {
  glViewport(0, 0, w, h);
  this->ar = (float)w / (float)h;
}

/* Draw the scene */
void OpenGLWidget::paintGL() {
  /* Set the background color */
  QColor bc = getBackgroundColor();
  glClearColor(bc.redF(), bc.greenF(), bc.blueF(), bc.alphaF());
  /* Clear the buffers */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  /* Set the projection matrix */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (projection_type == ProjectionType::PERSPECTIVE) {
    glFrustum(-0.5 * ar, 0.5 * ar, -0.5, 0.5, 1, 1000);
    glTranslatef(0, 0, -2 * ar);
  } else {
    static const float c = sqrt(3);  // because of the view cube with side 2
    glOrtho(-c * ar, c * ar, -c, c, -c, c);
  }

  /* Set the model-view matrix */
  glMatrixMode(GL_MODELVIEW);
  /* Perform all the transformations */
  // view rotation
  mat4 transform = OWVM_IDENTITY_INIT;
  owvm_rotate_x(transform, camera_rotx, transform);
  owvm_rotate_y(transform, camera_roty, transform);
  if (global_axes_enabled) {
    glLoadMatrixf((float *)transform);
    drawAxes();
  }
  // view scale
  //  owvm_scale_uni(transform, viewport_default_scale);
  // model translation
  owvm_translate(transform, (vec3){trnsx, trnsy, trnsz});
  // model rotation
  owvm_rotate_x(transform, rotx, transform);
  owvm_rotate_y(transform, roty, transform);
  owvm_rotate_z(transform, rotz, transform);
  // model scale
  owvm_scale_uni(transform, scaleu);
  owvm_scale(transform, (vec3){scalex, scaley, scalez});
  if (local_axes_enabled) {
    glLoadMatrixf((float *)transform);
    drawAxes();
  }
  // multiply with the model's normalisation matrix
  owvm_mul(transform, norm_matrix, transform);
  glLoadMatrixf((float *)transform);

  /* Draw the objects */
  if (!mesh)  // use a template model
    drawCubeScene();
  else
    drawObject(mesh);
}

void OpenGLWidget::drawCubeScene() { drawCube(0, 0, 0, 1); }

void OpenGLWidget::drawObject(const OWV_Mesh *m) {
  if (!m) return;

  /* Set up the buffers */
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, m->positions);

  /* Send the line draw calls */
  if (this->isLineDisplayEnabled()) {
    QColor lc = getLineColor();
    glColor3d(lc.redF(), lc.greenF(), lc.blueF());
    float lw = getLineWidth();
    glLineWidth(lw);
    if (getLineStyle() == LineStyle::DASHED) {
      glEnable(GL_LINE_STIPPLE);
      glLineStipple(10, 0x3333);
    }

    /* GL_LINE_LOOP glDrawElements in a loop variation */
    //    const unsigned int *index_offset = &m->indices[0];
    //    for (unsigned int i = 0; i < m->face_count; i++) {
    //      glDrawElements(GL_LINE_LOOP, m->face_vertex_counts[i],
    //      GL_UNSIGNED_INT,
    //                     index_offset);
    //      index_offset += m->face_vertex_counts[i];
    //    }

    /* GL_LINE_LOOP glMultiDrawElements variation */
    //    glMultiDrawElements(GL_LINE_LOOP, (GLsizei *)m->face_vertex_counts,
    //                        GL_UNSIGNED_INT, (GLvoid **)face_index_list,
    //                        m->face_count);

    /* GL_LINES glDrawElements variation */
    glDrawElements(GL_LINES, this->index_count, GL_UNSIGNED_INT,
                   this->index_array);

    glDisable(GL_LINE_STIPPLE);
  }

  /* Send the point draw calls */
  if (this->isPointDisplayEnabled()) {
    QColor pc = getPointColor();
    glColor3d(pc.redF(), pc.greenF(), pc.blueF());
    float ps = getPointSize();
    glPointSize(ps);
    if (getPointStyle() == PointStyle::CIRCLE) {
      glEnable(GL_POINT_SMOOTH);
    }

    glDrawArrays(GL_POINTS, 0, m->vertex_count);

    glDisable(GL_POINT_SMOOTH);
  }

  //  glDisableClientState(GL_VERTEX_ARRAY);
}

void OpenGLWidget::drawCube(float x, float y, float z, float side_len) {
  const OWV_Mesh *m = owv_mesh_create_cube(x, y, z, side_len);
  this->mesh_bounds = {.xcen = 0, .ycen = 0, .zcen = 0, .maxlen = side_len * 2};
  this->index_array = owv_iarr_to_unique_lines(m, &this->index_count);
  drawObject(m);
}

void OpenGLWidget::drawAxes() {
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
  static unsigned int index_count = 6;
  static unsigned int indices[] = {0, 1, 2, 3, 4, 5};

  glVertexPointer(3, GL_FLOAT, 0, positions);
  glLineWidth(1);
  QColor lc = "#9A9A9A";
  glColor3d(lc.redF(), lc.greenF(), lc.blueF());
  glDrawArrays(GL_LINES, 0, position_count);
}

/* Set interactive rotation in the viewport with a mouse */
void OpenGLWidget::mousePressEvent(QMouseEvent *m) { mouse_pos = m->pos(); }
void OpenGLWidget::mouseMoveEvent(QMouseEvent *m) {
  camera_rotx += camera_speed * (m->pos().y() - mouse_pos.y());
  camera_roty += camera_speed * (m->pos().x() - mouse_pos.x());
  mouse_pos = m->pos();
  update();
}

void OpenGLWidget::loadModel() {
  OWV_Mesh *m = owv_mesh_read_obj(this->file_name.c_str());
  if (this->mesh) {
    owv_mesh_destroy((OWV_Mesh *)this->mesh);
  }
  this->mesh = m;
  if (this->index_array) {
    free(this->index_array);
    index_count = 0;
  }
  this->mesh_bounds = owv_mesh_find_bounds(m);

  /* Break the index array of faces into the index array of UNIQUE lines */
  this->index_array = owv_iarr_to_unique_lines(m, &this->index_count);
  this->faces_count = m->face_count;
  this->vertices_count = m->vertex_count - 1;
  this->edges_count = this->index_count / 2;
  /* Break the index array of faces into the array of index arrays by faces */
  //  owv_index_arr_to_2d_arr(m);
  /* Break the index array of faces into the index array of lines */
  //  this->index_array = owv_to_lines_index_arr(m);
  //  this->index_count = m->index_count * 2;

  /* Create a normalization matrix for the model */
  const float maxlen = mesh_bounds.maxlen;
  const float divid = VIEWCUBE_SIDE;
  mat4 nm = OWVM_IDENTITY_INIT;
  OWV_MeshBounds &mb = this->mesh_bounds;
  owvm_scale(nm, (vec3){divid / maxlen, divid / maxlen, divid / maxlen});
  owvm_translate(nm, (vec3){-mb.xcen, -mb.ycen, -mb.zcen});
  memcpy(this->norm_matrix, nm, sizeof(nm));

  update();
}
