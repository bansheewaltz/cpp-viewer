#include "parsing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE (1 << 16)

/* Dynamic array implementaiton through macros */
// clang-format off
#define array_clean(_arr)      ((_arr) ? free(_array_header(_arr)) : 0)
#define array_push(_arr, _val) (_array_mgrow(_arr, 1) ? ((_arr)[_array_size(_arr)++] = (_val), _array_size(_arr) - 1) : 0)
#define array_size(_arr)       ((_arr) ? _array_size(_arr) : 0)
#define array_capacity(_arr)   ((_arr) ? _array_capacity(_arr) : 0)
#define array_empty(_arr)      (array_size(_arr) == 0)

#define _array_header(_arr)    ((unsigned int*)(_arr)-2)
#define _array_size(_arr)      (_array_header(_arr)[0])
#define _array_capacity(_arr)  (_array_header(_arr)[1])
#define _array_ngrow(_arr, _n) ((_arr) == 0 || (_array_size(_arr) + (_n) >= _array_capacity(_arr)))
#define _array_mgrow(_arr, _n) (_array_ngrow(_arr, _n) ? (_array_grow(_arr, _n) != 0) : 1)
#define _array_grow(_arr, _n)  (*((void**)&(_arr)) = array_realloc(_arr, _n, sizeof(*(_arr))))
// clang-format on

static void* array_realloc(void* arr, unsigned int n, size_t elem_size) {
  unsigned int size = array_size(arr);
  unsigned int nsize = size + n;
  unsigned int cap = array_capacity(arr);
  //  unsigned int ncap = cap + cap / 2;
  unsigned int ncap = cap * 2;

  if (ncap < nsize) {
    ncap = nsize;
  }

  ncap = (ncap + 15) & ~15u;

  size_t alloc_size = elem_size * ncap + 2 * sizeof(unsigned int);
  unsigned int* narr = realloc(arr ? _array_header(arr) : 0, alloc_size);
  if (!narr) {
    return 0;
  }

  narr[0] = size;
  narr[1] = ncap;

  return (narr + 2);
}

void parse_buffer(OWV_Mesh* m, const char* buffer, const char* last_nl);

OWV_Mesh* owv_mesh_read_obj(const char* path) {
  if (!path) {
    return 0;
  }

  FILE* file = fopen(path, "r");
  if (!file) {
    return 0;
  }

  OWV_Mesh* m = (calloc(1, sizeof(OWV_Mesh)));
  if (!m) {
    return 0;
  }

  /* Add dummy position for the convinience */
  array_push(m->positions, 0.0f);
  array_push(m->positions, 0.0f);
  array_push(m->positions, 0.0f);

  /* Create buffer for reading the file */
  char buffer[BUFFER_SIZE];

  unsigned int to_read = BUFFER_SIZE;
  unsigned int read = 0;
  unsigned int bytes_overflow = 0;
  char* start = buffer;  // is needed for cyclic buffer
  char* end;
  char* last_nl;

  while ((read = fread(start, 1, to_read, file))) {
    end = start + read;
    /* Find last new line */
    last_nl = end;
    while (last_nl > buffer) {
      last_nl--;
      if (*last_nl == '\n') {
        break;
      }
    }
    /* Check that there actually is a new line */
    if (*last_nl != '\n') {
      break;
    }
    last_nl++;
    /* Process buffer */
    parse_buffer(m, buffer, last_nl);
    /* Copy overflow for the next buffer */
    bytes_overflow = end - last_nl;
    memmove(buffer, last_nl, bytes_overflow);
    start = buffer + bytes_overflow;
    to_read = BUFFER_SIZE - bytes_overflow;
  }

  m->vertex_count = array_size(m->positions) / 3;
  m->face_count = array_size(m->face_vertex_counts);
  m->index_count = array_size(m->indices);

  fclose(file);

  return m;
}

static int is_whitespace(char c) { return c == ' ' || c == '\t' || c == '\r'; }
static int is_digit(char c) { return '0' <= c && c <= '9'; }
static int is_exponent(char c) { return c == 'e' || c == 'E'; }
static int is_newline(char c) { return c == '\n'; }

static const char* skip_whitespace(const char* p) {
  while (is_whitespace(*p)) {
    p++;
  }
  return p;
}

static const char* skip_line(const char* p) {
  while (!is_newline(*p)) {
    p++;
  }
  p++;
  return p;
}

#define MAX_POWER 19
static const double POWER_10_POS[] = {
    1.0e0,  1.0e1,  1.0e2,  1.0e3,  1.0e4,  1.0e5,  1.0e6,
    1.0e7,  1.0e8,  1.0e9,  1.0e10, 1.0e11, 1.0e12, 1.0e13,
    1.0e14, 1.0e15, 1.0e16, 1.0e17, 1.0e18, 1.0e19,
};
static const double POWER_10_NEG[] = {
    1.0e0,   1.0e-1,  1.0e-2,  1.0e-3,  1.0e-4,  1.0e-5,  1.0e-6,
    1.0e-7,  1.0e-8,  1.0e-9,  1.0e-10, 1.0e-11, 1.0e-12, 1.0e-13,
    1.0e-14, 1.0e-15, 1.0e-16, 1.0e-17, 1.0e-18, 1.0e-19,
};
static const char* parse_float(const char* ptr, float* val) {
  ptr = skip_whitespace(ptr);

  double sign;
  switch (*ptr) {
    case '+':
      sign = +1.0;
      ptr++;
      break;
    case '-':
      sign = -1.0;
      ptr++;
      break;
    default:
      sign = +1.0;
  }

  /* Parse the integral part */
  double num = 0.0;
  while (is_digit(*ptr)) {
    num = 10.0 * num + (double)(*ptr++ - '0');
  }

  if (*ptr == '.') ptr++;

  /* Parse the fractional part */
  double fnum = 0.0;
  double div = 1.0;
  while (is_digit(*ptr)) {
    fnum = 10.0 * fnum + (double)(*ptr++ - '0');
    div *= 10.0;
  }

  num += fnum / div;

  /* Parse the exponent if it is present */
  const double* powers;
  if (is_exponent(*ptr)) {
    ptr++;

    switch (*ptr) {
      case '+':
        powers = POWER_10_POS;
        ptr++;
        break;
      case '-':
        powers = POWER_10_NEG;
        ptr++;
        break;
      default:
        powers = POWER_10_POS;
    }

    unsigned int exp = 0;
    while (is_digit(*ptr)) {
      exp = 10 * exp + (*ptr++ - '0');
    }

    num *= (exp <= MAX_POWER ? powers[exp] : 0.0);
  }

  *val = (float)(sign * num);

  return ptr;
}

static const char* parse_vertex(OWV_Mesh* mesh, const char* ptr) {
  float val;
  for (int i = 0; i < 3; i++) {
    ptr = parse_float(ptr, &val);
    array_push(mesh->positions, val);
  }
  return ptr;
}

static const char* parse_int(const char* ptr, int* val) {
  ptr = skip_whitespace(ptr);

  int sign;
  if (*ptr == '-') {
    sign = -1;
    ptr++;
  } else {
    sign = +1;
  }

  int num = 0;
  while (is_digit(*ptr)) {
    num = 10 * num + (*ptr++ - '0');
  }

  *val = sign * num;

  return ptr;
}

static const char* parse_face(OWV_Mesh* mesh, const char* ptr) {
  unsigned int count = 0;

  int v;
  while (!is_newline(*ptr)) {
    ptr = parse_int(ptr, &v);

    if (v < 0)
      v = (array_size(mesh->positions) / 3) - (unsigned int)(-v);
    else
      v = (unsigned int)(v);

    array_push(mesh->indices, v);
    count++;

    // skip vertex- texture and normal indices
    while (!is_whitespace(*ptr) && !is_newline(*ptr)) {
      ptr++;
    }
    // in a case if there is a whitespace before the newline char
    ptr = skip_whitespace(ptr);
  }

  array_push(mesh->face_vertex_counts, count);

  return ptr;
}

void parse_buffer(OWV_Mesh* m, const char* buffer, const char* last_nl) {
  const char* it = buffer;
  while (it != last_nl) {
    switch (*it) {
      case 'v':
        if (is_whitespace(*++it)) {
          it = parse_vertex(m, it);
        }
        break;
      case 'f':
        it = parse_face(m, ++it);
        break;
    }
    it = skip_line(it);
  }
}

void owv_mesh_destroy(OWV_Mesh* m) {
  if (!m) return;
  array_clean(m->positions);
  array_clean(m->face_vertex_counts);
  array_clean(m->indices);
  free(m);
}

OWV_Mesh* owv_mesh_create_cube(float x, float y, float z, float side_len) {
  const float hside = side_len / 2;
  static const unsigned int vertex_count = 8;
  static float positions[24];
  const float instance_positions[24] = {
      x + hside, y - hside, z + hside,  // right bottom front
      x + hside, y - hside, z - hside,  // right bottom back
      x - hside, y - hside, z + hside,  // left bottom front
      x - hside, y - hside, z - hside,  // left bottom back

      x + hside, y + hside, z + hside,  // right top front
      x + hside, y + hside, z - hside,  // right top back
      x - hside, y + hside, z + hside,  // left top front
      x - hside, y + hside, z - hside,  // left top back
  };
  memcpy(positions, instance_positions, sizeof(float) * 24);
  static const unsigned int face_count = 6;
  static const unsigned int face_vertex_counts[] = {4, 4, 4, 4, 4, 4};
  static const unsigned int index_count = 24;
  static const unsigned int indices[] = {3, 2, 0, 1,   // bottom
                                         4, 5, 7, 6,   // top
                                         2, 3, 7, 6,   // left
                                         0, 1, 5, 4,   // right
                                         0, 4, 6, 2,   // near
                                         1, 3, 7, 5};  // far

  OWV_Mesh* m = (OWV_Mesh*)malloc(sizeof(OWV_Mesh));
  if (!m) return 0;
  *m = (OWV_Mesh){vertex_count, positions,
                  face_count,   (unsigned int*)face_vertex_counts,
                  index_count,  (unsigned int*)indices};
  return m;
}
