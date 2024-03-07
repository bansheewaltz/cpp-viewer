#ifndef PROCESSING_H_
#define PROCESSING_H_

#include <stddef.h>

#include "file_reader/lowlevel_file_reader.h"

#ifdef __cplusplus
extern "C" {
#endif

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

void owv_iarr_lines_sort(unsigned int* arr, size_t len);
unsigned int* owv_iarr_lines_clean(unsigned int* arr, size_t len,
                                   size_t* newlen);
void owv_iarr_lines_flip(unsigned int* arr, size_t len);

#ifdef __cplusplus
}
#endif

#endif  // PROCESSING_H_
