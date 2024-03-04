#ifndef DETAILS_H_
#define DETAILS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

void owv_iarr_lines_sort(unsigned int* arr, size_t len);
unsigned int* owv_iarr_lines_clean(unsigned int* arr, size_t len,
                                   size_t* newlen);
void owv_iarr_lines_flip(unsigned int* arr, size_t len);

#ifdef __cplusplus
}
#endif

#endif  // DETAILS_H_
