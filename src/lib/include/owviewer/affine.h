#include <math.h>

typedef float vec3[3];
typedef float vec4[4];
typedef vec4 mat4[4];

// clang-format off
#define OWVM_IDENTITY_INIT  {{1.0f, 0.0f, 0.0f, 0.0f}, \
                             {0.0f, 1.0f, 0.0f, 0.0f}, \
                             {0.0f, 0.0f, 1.0f, 0.0f}, \
                             {0.0f, 0.0f, 0.0f, 1.0f}}
// clang-format on

static inline void owvm_mul(mat4 m1, mat4 m2, mat4 dest) {
  float a00 = m1[0][0], a01 = m1[0][1], a02 = m1[0][2], a03 = m1[0][3],
        a10 = m1[1][0], a11 = m1[1][1], a12 = m1[1][2], a13 = m1[1][3],
        a20 = m1[2][0], a21 = m1[2][1], a22 = m1[2][2], a23 = m1[2][3],
        a30 = m1[3][0], a31 = m1[3][1], a32 = m1[3][2], a33 = m1[3][3],

        b00 = m2[0][0], b01 = m2[0][1], b02 = m2[0][2], b10 = m2[1][0],
        b11 = m2[1][1], b12 = m2[1][2], b20 = m2[2][0], b21 = m2[2][1],
        b22 = m2[2][2], b30 = m2[3][0], b31 = m2[3][1], b32 = m2[3][2],
        b33 = m2[3][3];

  dest[0][0] = a00 * b00 + a10 * b01 + a20 * b02;
  dest[0][1] = a01 * b00 + a11 * b01 + a21 * b02;
  dest[0][2] = a02 * b00 + a12 * b01 + a22 * b02;
  dest[0][3] = a03 * b00 + a13 * b01 + a23 * b02;

  dest[1][0] = a00 * b10 + a10 * b11 + a20 * b12;
  dest[1][1] = a01 * b10 + a11 * b11 + a21 * b12;
  dest[1][2] = a02 * b10 + a12 * b11 + a22 * b12;
  dest[1][3] = a03 * b10 + a13 * b11 + a23 * b12;

  dest[2][0] = a00 * b20 + a10 * b21 + a20 * b22;
  dest[2][1] = a01 * b20 + a11 * b21 + a21 * b22;
  dest[2][2] = a02 * b20 + a12 * b21 + a22 * b22;
  dest[2][3] = a03 * b20 + a13 * b21 + a23 * b22;

  dest[3][0] = a00 * b30 + a10 * b31 + a20 * b32 + a30 * b33;
  dest[3][1] = a01 * b30 + a11 * b31 + a21 * b32 + a31 * b33;
  dest[3][2] = a02 * b30 + a12 * b31 + a22 * b32 + a32 * b33;
  dest[3][3] = a03 * b30 + a13 * b31 + a23 * b32 + a33 * b33;
}

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
