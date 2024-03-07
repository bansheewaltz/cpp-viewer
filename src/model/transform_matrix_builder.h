#ifndef TRANSFORM_MATRIX_BUILDER_H_
#define TRANSFORM_MATRIX_BUILDER_H_

#include <cmath>

#include "transform_matrix.h"

class TransformMatrixBuilder {
  static TransformMatrix CreateRotationMatrix(float x, float y, float z) {
    TransformMatrix matx;
    float cosx = cosf(to_rad(x));
    float sinx = sinf(to_rad(x));
    matx[1][1] = +cosx;
    matx[1][2] = +sinx;
    matx[2][1] = -sinx;
    matx[2][2] = +cosx;

    TransformMatrix maty;
    float cosy = cosf(to_rad(y));
    float siny = sinf(to_rad(y));
    maty[0][0] = +cosy;
    maty[0][2] = -siny;
    maty[2][0] = +siny;
    maty[2][2] = +cosy;

    TransformMatrix matz;
    float cosz = cosf(to_rad(z));
    float sinz = sinf(to_rad(z));
    matz[0][0] = +cosz;
    matz[0][1] = +sinz;
    matz[1][0] = -sinz;
    matz[1][1] = +cosz;

    return matx * maty * matz;
  }

  static TransformMatrix CreateTranslationMatrix(float x, float y, float z) {
    TransformMatrix res;
    for (int i = 0; i < 4; i++) {
      res[3][i] += res[0][i] * x;
      res[3][i] += res[1][i] * y;
      res[3][i] += res[2][i] * z;
    }
    return res;
  }

  static TransformMatrix CreateScaleMatrix(float x, float y, float z) {
    TransformMatrix res;
    for (int i = 0; i < 4; i++) {
      res[0][i] *= x;
      res[1][i] *= y;
      res[2][i] *= z;
    }
    return res;
  }

  static inline double to_rad(double deg) { return deg * (M_PI / 180); }
};

#endif  // TRANSFORM_MATRIX_BUILDER_H_
