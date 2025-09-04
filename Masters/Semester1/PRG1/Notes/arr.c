#include <stdio.h>

int* decompose() {
  int i = 10;

  return &i;
}

int main() {
  int* a = decompose();
  printf("%d\n", *a);
}
