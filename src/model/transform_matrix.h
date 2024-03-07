#ifndef TRANSFORM_MATRIX_H_
#define TRANSFORM_MATRIX_H_

class TransformMatrix {
 public:
  TransformMatrix operator*(const TransformMatrix& m2) {
    TransformMatrix res;
    auto m1 = *this;

    for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++)
        for (int k = 0; k < 4; k++)
          res.data_[i][j] += m1.data_[i][k] * m2.data_[k][j];

    return res;
  }

  float* operator[](int row) { return data_[row]; }

 private:
  float data_[4][4] = {{1.0f, 0.0f, 0.0f, 0.0f},  // identitiy matrix
                       {0.0f, 1.0f, 0.0f, 0.0f},
                       {0.0f, 0.0f, 1.0f, 0.0f},
                       {0.0f, 0.0f, 0.0f, 1.0f}};
};

#endif  // TRANSFORM_MATRIX_H_
