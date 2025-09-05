#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char str1[40] = "Yes";
  const char *str2 = "Yes";
  const char *str3 = "World!";
  char dst[4];
  printf("%d\n", compara_str(str1, str2));
  printf("%d\n", strcmp(str1, str2));
  printf("%d\n", concatena_str(str1, str2, str3));

  const char *str_src = "twotwo two three";
  const char *str_find = "two";

  // printf("%d\n", encuentra_str(str_find, str_src));
  const char *str_freq = "adslfalksfjlqwerqwr";

  int **freq = frecuencia(str_freq);

  for (int j = 0; j < 26; j++) {
    if (*(*(freq + j) + 1) == 0)
      continue;
    printf("%c - %d\n", *(*(freq + j)), *(*(freq + j)+1));
  }
  free(freq);

  sin_repetir(str_src);
  return 0;
}
