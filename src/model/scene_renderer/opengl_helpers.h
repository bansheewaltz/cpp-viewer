#ifndef OPENGL_HELPERS_
#define OPENGL_HELPERS_

#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <csignal>  // NOLINT
#include <iostream>

#define ASSERT(x) \
  if (!(x)) std::raise(SIGTRAP);
#define GLCall(x) \
  GLClearError(); \
  x;              \
  ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError() {
  while (glGetError() != GL_NO_ERROR)
    ;
}
static bool GLLogCall(const char *function, const char *file, int line) {
  if (GLenum error = glGetError()) {
    std::cout << "OpenGL error: " << std::hex << error << std::dec << "\n"  //
              << "call: " << function << "\n"
              << "at: " << file << ":" << line << std::endl;
    return false;
  }
  return true;
}

#endif  // OPENGL_HELPERS_
