#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  unsigned int n_bytes = 0;
  unsigned int n_lines = 0;
  unsigned int n_letters = 0;
  if (argc != 2) {
    fprintf(stderr, "Invalid argument count. Usage: %s <file>", argv[0]);
    exit(1);
  }
  FILE *file = fopen(argv[1], "r");
  if (!file) {
    perror("Could not open file.\n");
    exit(1);
  }
  char c, previous_char = 0;
  int *frequency = calloc(26, sizeof(int));
  int *c_after_e = calloc(26, sizeof(int));
  char is_e = 0;
  while ((c = fgetc(file)) != EOF) {
    n_bytes++;
    if (c == '\n') {
      n_lines++;
    }
    if (isalpha(c)) {
      char lower_c = tolower(c);
      frequency[lower_c - 'a']++;
      n_letters++;

      if (tolower(previous_char) == 'e') {
        c_after_e[lower_c - 'a']++;
      }
    }
    previous_char = c;
  }

  int e_frequency = frequency['e' - 'a'];

  printf("File has %d bytes\n", n_bytes);
  printf("File has %d lines\n", n_lines);
    printf("======================================================================\n");
    printf(" char\t\tfreq.\t\tpbb(char)\t\t pbb(char|e)\n");
    printf("======================================================================\n");
  for (int i = 0; i < 26; i++) {
    float pbb = (float)frequency[i]/n_letters;
    float pbb_e = (float)c_after_e[i] / e_frequency;
    printf(" %c\t\t%d\t\t%f\t\t%f\n", 'a' + i, frequency[i], pbb, pbb_e);
  }
  free(frequency);
  free(c_after_e);
  fclose(file);
}
