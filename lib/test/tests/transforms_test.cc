#include "cglm/cglm.h"
#include "gtest/gtest.h"
#include "transform_matrix.h"
#include "transform_matrix_builder.h"

TEST(MatrixTransformTest, SeriesOfTransformation) {
  const float maxlen = 2;
  const float divid = 2;
  const float xcen = 1, ycen = 2, zcen = 3;
  const float rotx = 30, roty = 45, rotz = 60;
  const float trnsx = 1, trnsy = 2, trnsz = 3;
  const float scalex = 1.1, scaley = 1.2, scalez = 1.3, scaleu = 0.5;

  TransformMatrix nm;
  nm = nm * TransformMatrixBuilder::CreateScaleMatrix(
                divid / maxlen, divid / maxlen, divid / maxlen);
  nm =
      nm * TransformMatrixBuilder::CreateTranslationMatrix(-xcen, -ycen, -zcen);

  mat4 nm2 = GLM_MAT4_IDENTITY_INIT;
  glm_scale(nm2, (vec3){divid / maxlen, divid / maxlen, divid / maxlen});
  glm_translate(nm2, (vec3){-xcen, -ycen, -zcen});

  TransformMatrix res;
  mat4 ref = GLM_MAT4_IDENTITY_INIT;

  res = res * TransformMatrixBuilder::CreateRotationMatrix(rotx, 0, 0);
  glm_rotate_x(ref, glm_rad(rotx), ref);
  for (int i = 0; i < 16; i++)
    EXPECT_EQ(res.data()[i], ((float*)ref)[i]) << "rotx";

  res = res * TransformMatrixBuilder::CreateRotationMatrix(0, roty, 0);
  glm_rotate_y(ref, glm_rad(roty), ref);
  for (int i = 0; i < 16; i++)
    EXPECT_EQ(res.data()[i], ((float*)ref)[i]) << "roty";

  res = res *
        TransformMatrixBuilder::CreateTranslationMatrix(trnsx, trnsy, trnsz);
  glm_translate(ref, (vec3){trnsx, trnsy, trnsz});
  for (int i = 0; i < 16; i++)
    EXPECT_EQ(res.data()[i], ((float*)ref)[i]) << "tr";

  res = res * TransformMatrixBuilder::CreateRotationMatrix(rotx, 0, 0);
  glm_rotate_x(ref, glm_rad(rotx), ref);
  for (int i = 0; i < 16; i++)
    EXPECT_EQ(res.data()[i], ((float*)ref)[i]) << "rotx";

  res = res * TransformMatrixBuilder::CreateRotationMatrix(0, roty, 0);
  glm_rotate_y(ref, glm_rad(roty), ref);
  for (int i = 0; i < 16; i++)
    EXPECT_EQ(res.data()[i], ((float*)ref)[i]) << "roty";

  res = res * TransformMatrixBuilder::CreateRotationMatrix(0, 0, rotz);
  glm_rotate_z(ref, glm_rad(rotz), ref);
  for (int i = 0; i < 16; i++)
    EXPECT_EQ(res.data()[i], ((float*)ref)[i]) << "rotz";

  res = res * TransformMatrixBuilder::CreateScaleMatrix(1, 1, 1, scaleu);
  glm_scale_uni(ref, scaleu);
  for (int i = 0; i < 16; i++)
    EXPECT_EQ(res.data()[i], ((float*)ref)[i]) << "uni";

  res = res * TransformMatrixBuilder::CreateScaleMatrix(scalex, scaley, scalez);
  glm_scale(ref, (vec3){scalex, scaley, scalez});
  for (int i = 0; i < 16; i++)
    EXPECT_EQ(res.data()[i], ((float*)ref)[i]) << "scale";

  res = res * nm;
  glm_mul(ref, nm2, ref);
  for (int i = 0; i < 16; i++) EXPECT_EQ(res.data()[i], ((float*)ref)[i]);
}
