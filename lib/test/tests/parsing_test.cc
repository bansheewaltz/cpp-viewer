#include "gtest/gtest.h"
#include "lowlevel_parser.h"

TEST(ParserTest, Normal) {
  const char *path = "/Users/mark/Downloads/3Dmodels/Lion.obj";
  OWV_Mesh *m = owv_mesh_read_obj(path);
  EXPECT_NE(m, (OWV_Mesh *)0);
  owv_mesh_destroy(m);
}

TEST(ParserTest, EmptyPath) {
  OWV_Mesh *m = owv_mesh_read_obj("");
  EXPECT_EQ(m, (OWV_Mesh *)0);
}

TEST(ParserTest, NoFile) {
  OWV_Mesh *m = owv_mesh_read_obj("a");
  EXPECT_EQ(m, (OWV_Mesh *)0);
}
