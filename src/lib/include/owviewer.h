#ifndef OWVIEWER_H_
#define OWVIEWER_H_

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Parsing of .obj file */

typedef struct {
  /* Vertex data: three coordinates for each vertex */
  unsigned int vertex_count;
  float* positions;

  /* Face data: one element for each face */
  unsigned int face_count;
  unsigned int* face_vertex_counts;

  /* Index data: one element for each face vertex */
  unsigned int index_count;
  unsigned int* indices;

} OWV_Mesh;

OWV_Mesh* owv_mesh_read_obj(const char* path);
void owv_mesh_destroy(OWV_Mesh* mesh);
// Mesh example
OWV_Mesh* owv_mesh_create_cube(float x, float y, float z, float side_len);

/* Processing of geometry data */

typedef struct {
  float xmin, ymin, zmin;
  float xmax, ymax, zmax;
  float xcen, ycen, zcen;
  float xlen, ylen, zlen;
  float maxlen;
} OWV_MeshBounds;

OWV_MeshBounds owv_mesh_find_bounds(const OWV_Mesh* mesh);

unsigned int* owv_iarr_to_lines(const OWV_Mesh* m);
unsigned int* owv_iarr_to_unique_lines(const OWV_Mesh* m, size_t* newlen);
unsigned int** owv_iarr_to_2d_iarr(const OWV_Mesh* m);

/* Affine transformations */

#include "owviewer/affine.h"

#ifdef __cplusplus
}
#endif

#endif  // OWVIEWER_H_
