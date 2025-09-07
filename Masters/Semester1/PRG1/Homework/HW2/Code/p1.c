#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// int main() {
//   char str1[40] = "Yes";
//   const char *str2 = "Yes";
//   const char *str3 = "World!";
//   char dst[4];
//   printf("%d\n", compara_str(str1, str2));
//   printf("%d\n", strcmp(str1, str2));
//   printf("%d\n", concatena_str(str1, str2, str3));
//
//   const char *str_src = "twotwo two three";
//   const char *str_find = "two";
//
//   // printf("%d\n", encuentra_str(str_find, str_src));
//   const char *str_freq = "adslfalksfjlqwerqwr";
//
//   int **freq = frecuencia(str_freq);
//
//   for (int j = 0; j < 26; j++) {
//     if (*(*(freq + j) + 1) == 0)
//       continue;
//     printf("%c - %d\n", *(*(freq + j)), *(*(freq + j)+1));
//   }
//   free(freq);
//
//   sin_repetir(str_src);
//   return 0;
// }

int main(void) {
  // Example for longitud_str
  const char *s1 = "hello world";
  printf("Length of \"%s\": %d\n", s1, longitud_str(s1));
  printf("--------------------------------------\n");

  // Example for copia_str
  char dest_buffer[20];
  const char *src_string = "Hello";
  copia_str(dest_buffer, src_string, sizeof(dest_buffer));
  printf("Copied string: %s\n", dest_buffer);
  printf("--------------------------------------\n");

  // Example for compara_str
  const char *str_a = "apple";
  const char *str_b = "apple";
  const char *str_c = "apply";
  printf("Comparing \"%s\" and \"%s\": %d\n", str_a, str_b, compara_str(str_a, str_b));
  printf("Comparing \"%s\" and \"%s\": %d\n", str_a, str_c, compara_str(str_a, str_c));
  printf("--------------------------------------\n");

  // Example for concatena_str
  char concat_buffer[50] = "First";
  const char *str_to_add1 = "Second";
  const char *str_to_add2 = "Third";
  int new_len = concatena_str(concat_buffer, str_to_add1, str_to_add2);
  printf("Concatenated string: \"%s\" with length: %d\n", concat_buffer, new_len);
  printf("--------------------------------------\n");

  // Example for encuentra_str
  const char *haystack = "ababababa";
  const char *needle = "aba";
  int occurrences = encuentra_str(needle, haystack);
  printf("Occurrences of \"%s\" in \"%s\": %d\n", needle, haystack, occurrences);
  printf("--------------------------------------\n");

  // Example for frecuencia
  const char *freq_str = "programming";
  int **freq_result = frecuencia(freq_str);
  printf("Character frequencies for \"%s\":\n", freq_str);
  for (int i = 0; i < 26; i++) {
    if (freq_result[i][1] > 0) {
      printf("  %c —  %d\n", (char)freq_result[i][0], freq_result[i][1]);
    }
    free(freq_result[i]);
  }
  free(freq_result);
  printf("--------------------------------------\n");
  
  // Example for sin_repetir
  const char *unique_str = "hello world apple banana";
  printf("Words with no repeated characters in \"%s\":\n", unique_str);
  sin_repetir(unique_str);
  
  return 0;
}
