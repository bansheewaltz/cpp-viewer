#include <math.h>

typedef float vec3[3];
typedef float vec4[4];
typedef vec4 mat4[4];

// clang-format off
#define OWVM_IDENTITY_INIT  {{1.0f, 0.0f, 0.0f, 0.0f}, \
                             {0.0f, 1.0f, 0.0f, 0.0f}, \
                             {0.0f, 0.0f, 1.0f, 0.0f}, \
                             {0.0f, 0.0f, 0.0f, 1.0f}}

static inline void owvm_mul(mat4 a, mat4 b, mat4 dest) {
  dest[0][0] = a[0][0] * b[0][0] + a[1][0] * b[0][1] + a[2][0] * b[0][2];
  dest[0][1] = a[0][1] * b[0][0] + a[1][1] * b[0][1] + a[2][1] * b[0][2];
  dest[0][2] = a[0][2] * b[0][0] + a[1][2] * b[0][1] + a[2][2] * b[0][2];
  dest[0][3] = a[0][3] * b[0][0] + a[1][3] * b[0][1] + a[2][3] * b[0][2];
  
  dest[1][0] = a[0][0] * b[1][0] + a[1][0] * b[1][1] + a[2][0] * b[1][2];
  dest[1][1] = a[0][1] * b[1][0] + a[1][1] * b[1][1] + a[2][1] * b[1][2];
  dest[1][2] = a[0][2] * b[1][0] + a[1][2] * b[1][1] + a[2][2] * b[1][2];
  dest[1][3] = a[0][3] * b[1][0] + a[1][3] * b[1][1] + a[2][3] * b[1][2];
  
  dest[2][0] = a[0][0] * b[2][0] + a[1][0] * b[2][1] + a[2][0] * b[2][2];
  dest[2][1] = a[0][1] * b[2][0] + a[1][1] * b[2][1] + a[2][1] * b[2][2];
  dest[2][2] = a[0][2] * b[2][0] + a[1][2] * b[2][1] + a[2][2] * b[2][2];
  dest[2][3] = a[0][3] * b[2][0] + a[1][3] * b[2][1] + a[2][3] * b[2][2];
  
  dest[3][0] = a[0][0] * b[3][0] + a[1][0] * b[3][1] + a[2][0] * b[3][2] + a[3][0] * b[3][3];
  dest[3][1] = a[0][1] * b[3][0] + a[1][1] * b[3][1] + a[2][1] * b[3][2] + a[3][1] * b[3][3];
  dest[3][2] = a[0][2] * b[3][0] + a[1][2] * b[3][1] + a[2][2] * b[3][2] + a[3][2] * b[3][3];
  dest[3][3] = a[0][3] * b[3][0] + a[1][3] * b[3][1] + a[2][3] * b[3][2] + a[3][3] * b[3][3];
}
// clang-format on

static inline double to_radians(double degrees) {
  return degrees * (M_PI / 180);
}

static inline void owvm_rotate_x(mat4 m, float angle, mat4 dest) {
  mat4 t = OWVM_IDENTITY_INIT;
  float c = cosf(to_radians(angle));
  float s = sinf(to_radians(angle));

  t[1][1] = +c;
  t[1][2] = +s;
  t[2][1] = -s;
  t[2][2] = +c;

  owvm_mul(m, t, dest);
}

static inline void owvm_rotate_y(mat4 m, float angle, mat4 dest) {
  mat4 t = OWVM_IDENTITY_INIT;
  float c = cosf(to_radians(angle));
  float s = sinf(to_radians(angle));

  t[0][0] = +c;
  t[0][2] = -s;
  t[2][0] = +s;
  t[2][2] = +c;

  owvm_mul(m, t, dest);
}

static inline void owvm_rotate_z(mat4 m, float angle, mat4 dest) {
  mat4 t = OWVM_IDENTITY_INIT;
  float c = cosf(to_radians(angle));
  float s = sinf(to_radians(angle));

  t[0][0] = +c;
  t[0][1] = +s;
  t[1][0] = -s;
  t[1][1] = +c;

  owvm_mul(m, t, dest);
}

static inline void owvm_scale(mat4 m, vec3 v) {
  for (int i = 0; i < 4; i++) {
    m[0][i] *= v[0];
    m[1][i] *= v[1];
    m[2][i] *= v[2];
  }
}

static inline void owvm_scale_uni(mat4 m, float f) {
  for (int i = 0; i < 4; i++) {
    m[0][i] *= f;
    m[1][i] *= f;
    m[2][i] *= f;
  }
}

static inline void owvm_translate(mat4 m, vec3 v) {
  for (int i = 0; i < 4; i++) {
    m[3][i] += m[0][i] * v[0];
    m[3][i] += m[1][i] * v[1];
    m[3][i] += m[2][i] * v[2];
  }
}
