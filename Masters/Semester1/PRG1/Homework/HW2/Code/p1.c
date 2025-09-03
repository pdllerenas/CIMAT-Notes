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
  char* init = destiny;

  if ((destiny < source + longitud_str(source) && destiny > source) ||
      (destiny < source && source < destiny + longitud_str(destiny))) {
    fprintf(stderr, "Strings overlap in memory. Exiting program.\n");
    exit(1);
  }
  do {
    *destiny = *source;
  }
  while (*(source++) | *(destiny++));
  return init;
}

int compara_str(const char* str1, const char* str2) {
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
  char const *str1_copy = str1;
  while (*(str1++));
  str1--;
  *str1 = ' ';

  str1++;
  do {
    *(str1++) = *(str2++);
  } while(*str2);

  *str1 = ' ';
  str1++;

  do {
    *(str1++) = *(str3++);
  } while(*str3);

  *str1 = '\0';
 
  return longitud_str(str1_copy);
}

int encuentra_str(const char *str1, const char *str2) {
  
}

