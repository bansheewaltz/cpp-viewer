#ifndef PARSING_H_
#define PARSING_H_

#include <stdbool.h>

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

#ifdef __cplusplus
}
#endif

#endif  // PARSING_H_
