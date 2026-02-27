#include "compare.h"

int compare_int(const void *a, const void *b) {
  int int_a = *(int *)a;
  int int_b = *(int *)b;
  return (int_a - int_b);
}

int compare_uchar(const void *a, const void *b) {
  unsigned char uchar_a = *(unsigned char *)a;
  unsigned char uchar_b = *(unsigned char *)b;
  return (uchar_a - uchar_b);
}
