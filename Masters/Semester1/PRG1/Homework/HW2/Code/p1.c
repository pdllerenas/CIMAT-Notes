#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int longitud_str(const char *);
char *copia_str(char *, const char *);
int compara_str(const char *, const char *);
int concatena_str(char *, const char *, const char *);
int encuentra_str(const char *, const char *);
int **frecuencia(const char *);
char *sin_repetir(const char *);

int main() {

  char str1[40] = "Yes,";
  const char *str2 = "Hello,";
  const char *str3 = "World!";
  char dst[4];
  printf("%d\n", compara_str(str1, str2));
  printf("%d\n", strcmp(str1, str2));
  printf("%d\n", concatena_str(str1, str2, str3));

  const char *str_src = "twotwo two three";
  const char *str_find = "two";

  // printf("%d\n", encuentra_str(str_find, str_src));
  const char *str_freq = "adslfalksfjlqwerqwr";

  int(*freq)[2] = frecuencia(str_src);

  for (int j = 0; j < 26; j++) {
    if (freq[j][1] == 0)
      continue;
    printf("%c - %d\n", freq[j][0], freq[j][1]);
  }
  free(freq);

  sin_repetir(str_src);
  return 0;
}

int longitud_str(const char *str) {
  int len = 0;
  while (*(str++)) {
    len++;
  }
  return len;
}

char *copia_str(char *destiny, const char *source) {
  char *init = destiny;

  if ((destiny < source + longitud_str(source) && destiny > source) ||
      (destiny < source && source < destiny + longitud_str(destiny))) {
    fprintf(stderr, "Strings overlap in memory. Exiting program.\n");
    exit(1);
  }
  do {
    *destiny = *source;
  } while (*(source++) | *(destiny++));
  return init;
}

int compara_str(const char *str1, const char *str2) {
  do {
    if (*str1 > *str2) {
      return *str1 - *str2;
    }
    if (*str1 < *str2) {
      return *str1 - *str2;
    }
  } while (*(str1++) | *(str2++));
  return *str1 - *str2;
}

int concatena_str(char *str1, const char *str2, const char *str3) {
  char const *str1_copy = str1; // const pointer to start of str1, we will
                                // return this after all concatenations are done

  // set pointer to null character of str1
  while (*(str1++))
    ;
  str1--; // go back so we can replace null character with space char
  *str1 = ' ';

  str1++;
  do { // set characters on str1 until null char of str2 is reached
    *(str1++) = *(str2++);
  } while (*str2);

  *str1 = ' ';
  str1++;

  do {
    *(str1++) = *(str3++);
  } while (*str3);

  *str1 = '\0'; // end str1 with null character so that we can print it without
                // having memory problems

  // return length using pointer to start of str1
  return longitud_str(str1_copy);
}

int encuentra_str(const char *str1, const char *str2) {
  if (*str1 == '\0' ||
      *str2 == '\0') // If either of the strings are empty, return 0
    return 0;
  int occurrences = 0; // occurrence counter
  char const *cpy_str1 =
      str1; // constant pointer to where str1 starts, so we may reset str1
            // pointer after making a character match in the following nested
            // while loop
  while (*str2) { // while str2 does not point to the null terminating
                  // character, we continue searching for occurrences of str1

    // this initializes a loop once we find that the first
    // character of str1 matches some character of str2
    while (*str1 == *str2) {
      str1++;
      str2++;
      // if we reach the end of str1, we have found one occurrence
      if (*str1 == '\0') {
        occurrences++;
        str2--; // go back a position in str2, as we will add it back when
                // exiting this loop
        str1 = cpy_str1; // reset str1 pointer position for next possible
                         // character match
        break;
      }
    }
    str2++;
  }
  return occurrences;
}

int **frecuencia(const char *str) {
  if (*str == '\0')
    return 0;
  int(*freq)[2] =
      malloc(26 * sizeof(*freq)); // assuming 26 lower-case letters a-z (no ~n)

  for (int i = 0; i < 26; i++) {
    freq[i][0] = 'a' + i;
    freq[i][1] = 0;
  }

  while (*str) {
    if (*str - 'a' >= 0 && *str - 'a' < 26)
      freq[*str - 'a'][1] += 1;
    str++;
  }

  return freq;
}

char *sin_repetir(const char *str) {
  char word[30];
  char *ptr = word;

  while (*str) {
    *ptr = *str;
    ptr++;
    str++;
    if (*str == ' ' || *str == '\0') {
      *ptr = '\0';
      ptr = word;
      int(*freq)[2] = frecuencia(ptr);
      for (int i = 0; i < 26; i++) {
        if (freq[i][1] > 1)
          break;
        if (i == 25) {
          printf("%s\n", word);
        }
      }
      ptr = word;
    }
  }
  return ptr;
}
