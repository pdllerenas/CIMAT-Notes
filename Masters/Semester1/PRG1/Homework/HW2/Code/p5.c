#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int repetitions(char *str, char ch) {
  int ch_count = 0;
  while (*str) {
    if (*str == ch)
      ch_count++;
    str++;
  }
  return ch_count;
}

char **tokenize(char *str, char ch) {
  int ch_rep = repetitions(str, ch);
  char **token_list = malloc((ch_rep + 1) * sizeof(*token_list));
  char temp_token[100];
  int token_len = 0, token_count = 0;
  do {
    if (*str == ch || *str == '\0') {
      char *token = malloc(token_len * sizeof(char) + 1);
      temp_token[token_len] = '\0';
      strcpy(token, temp_token);
      *(token_list + token_count) = token;
      token_count++;
      token_len = 0;
    } else {
      temp_token[token_len] = *str;
      token_len++;
    }
    str++;
  } while (token_count <= ch_rep);
  return token_list;
}

int main() {
  char *str = "Hello one two three four five dd";
  char tkn = ' ';
  char **tokenized = tokenize(str, tkn);
  for (int i = 0; i < 7; i++) {
    printf("%s\n", *(tokenized + i));
  }
  free(tokenized);
}
