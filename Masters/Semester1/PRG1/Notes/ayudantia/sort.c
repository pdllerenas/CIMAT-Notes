#include <stdio.h>
#include <stdlib.h>

int *sort(int *a, int size_a, int *b, int size_b) {
  int *sorted = malloc((size_a + size_b)*sizeof(int));
  int count = 0;
  int i,j;
  for (i = 0, j = 0; i < size_a && j < size_b;) {
    if (a[i] < b[j])   {
      sorted[count++] = a[i];
      i++;
    }
    else {
      sorted[count++] = b[j];
      j++;
    }
  }
  if (i < j) {
    while (count < size_a + size_b)  {
      sorted[count++] = a[i++];
    }
  }
  if (j < i) {
    while (count < size_a + size_b)  {
      sorted[count++] = b[j++];
    }
  }
  return sorted;
}

int main() {
  int a[3] = {4,5,9};
  int b[3] = {3,4,8};
  int* sorted = sort(a, 3, b, 3);
  for (int i = 0; i < 6; i++) {
    printf("%d\n", sorted[i]);
  }
  free(sorted);
}
