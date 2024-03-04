#include <float.h>
#include <stdlib.h>

#include "details.h"
#include "owviewer.h"

OWV_MeshBounds owv_mesh_find_bounds(const OWV_Mesh* mesh) {
  float xmax = FLT_MIN, ymax = FLT_MIN, zmax = FLT_MIN;
  float xmin = FLT_MAX, ymin = FLT_MAX, zmin = FLT_MAX;

  float* p = mesh->positions;
  for (int i = 3; i < mesh->vertex_count * 3; i += 3) {
    if (p[i + 0] > xmax) xmax = p[i + 0];
    if (p[i + 0] < xmin) xmin = p[i + 0];
    if (p[i + 1] > ymax) ymax = p[i + 1];
    if (p[i + 1] < ymin) ymin = p[i + 1];
    if (p[i + 2] > zmax) zmax = p[i + 2];
    if (p[i + 2] < zmin) zmin = p[i + 2];
  }

  float xcen = (xmin + xmax) / 2.0f;
  float ycen = (ymin + ymax) / 2.0f;
  float zcen = (zmin + zmax) / 2.0f;

  float xlen = (xmax - xmin);
  float ylen = (ymax - ymin);
  float zlen = (zmax - zmin);

  float maxlen = xlen;
  if (maxlen < ylen) maxlen = ylen;
  if (maxlen < zlen) maxlen = zlen;

  return (OWV_MeshBounds){
      xmin,   ymin, zmin,  //
      xmax,   ymax, zmax,  //
      xcen,   ycen, zcen,  //
      xlen,   ylen, zlen,  //
      maxlen,
  };
}

unsigned int* owv_iarr_to_lines(const OWV_Mesh* m) {
  unsigned int* farr = m->indices;
  unsigned int* larr = malloc(2 * sizeof(unsigned int) * m->index_count);

  unsigned int* face_indices = farr;
  size_t i = 0;
  for (int j = 0; j < m->face_count; j++) {
    size_t face_vertex_count = m->face_vertex_counts[j];
    for (int k = 0; k < face_vertex_count; k++) {
      larr[i++] = face_indices[k];
      if (k == 0) continue;
      if (k == face_vertex_count - 1) {
        larr[i++] = face_indices[k];
        larr[i++] = face_indices[0];
      } else {
        larr[i++] = face_indices[k];
      }
    }
    face_indices += face_vertex_count;
  }

  return larr;
}

typedef struct {
  unsigned int s;
  unsigned int e;
} LineStartEnd;

static int cmpfunc(const void* a, const void* b) {
  const LineStartEnd* l1 = (LineStartEnd*)a;
  const LineStartEnd* l2 = (LineStartEnd*)b;
  int diff;
  diff = l1->s - l2->s;
  if (l1->s - l2->s != 0) {
    return diff;
  }
  diff = l1->e - l2->e;
  return diff;
}

void owv_iarr_lines_sort(unsigned int* arr, size_t len) {
  qsort(arr, len / 2, sizeof(unsigned int) * 2, &cmpfunc);
}

static bool is_pair_eq(unsigned int* a, unsigned int* b) {
  return *a == *b && *(a + 1) == *(b + 1);
}

unsigned int* owv_iarr_lines_clean(unsigned int* arr, size_t len,
                                   size_t* newlen) {
  if (len <= 4) return arr;
  // We will work with ptr to start index of each line
  unsigned int* ref = &arr[0];  // reference line start index
  unsigned int* cur = ref + 2;  // current line start index
  unsigned int* gap = NULL;     // gap start index

  while (cur != &arr[len - 1 - 1]) {
    if (is_pair_eq(ref, cur)) {
      if (!gap) gap = cur;
    } else {
      if (gap) {
        *gap = *cur, *(gap + 1) = *(cur + 1);
        ref = gap;
        gap += 2;
      } else {
        ref = cur;
      }
    }
    cur += 2;
  }

  *newlen = gap - arr;
  size_t alloc_size = *newlen * sizeof(unsigned int);
  arr = (unsigned int*)realloc(arr, alloc_size);
  if (!arr) return 0;

  return arr;
}

void owv_iarr_lines_flip(unsigned int* arr, size_t len) {
  if (!arr) return;
  for (int i = 0; i < len; i += 2) {
    if (arr[i] > arr[i + 1]) {
      unsigned int t = arr[i];
      arr[i] = arr[i + 1];
      arr[i + 1] = t;
    }
  }
}

unsigned int* owv_iarr_to_unique_lines(const OWV_Mesh* m, size_t* newlen) {
  unsigned int* arr = owv_iarr_to_lines(m);
  owv_iarr_lines_flip(arr, m->index_count * 2);
  owv_iarr_lines_sort(arr, m->index_count * 2);
  arr = owv_iarr_lines_clean(arr, m->index_count * 2, newlen);
  return arr;
}

unsigned int** owv_iarr_to_2d_iarr(const OWV_Mesh* m) {
  unsigned int** arr = (unsigned int**)(m->face_count * sizeof(unsigned int*));
  if (!arr) return 0;
  unsigned int* index_offset = &m->indices[0];
  for (int i = 0; i < m->face_count; i++) {
    arr[i] = index_offset;
    index_offset += m->face_vertex_counts[i];
  }
  return arr;
}
