#include <stdio.h>
#include <stdlib.h>

int repetitions(char *str, char ch) {
  int ch_count = 0;
  while (*str) {
    if (*str == ch) ch_count++;
  }
  return ch_count;
}

char **tokenize(char *str, char ch) {
  int ch_rep = repetitions(str, ch);
  char **tokens = malloc(ch_rep*sizeof(*tokens));
  while (*str) {
    printf("%c\n", *str);
    **tokens = *str;

    if (*str == ch) {
      tokens++;
    }
    str++;
  }
  return tokens;
}

int main() {
  char** tokenized = tokenize("Hello one two three four five", ' ');
  for (int i = 0; i < 6; i++) {
    printf("%s\n", *(tokenized+i));
  }
}
