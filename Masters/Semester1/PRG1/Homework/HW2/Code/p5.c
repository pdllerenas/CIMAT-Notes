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
  int ch_rep = repetitions(str, ch); // number of tokens in str

  // make space for ch_rep+1 tokenized words
  char **token_list = malloc((ch_rep + 1) * sizeof(*token_list));
  char temp_token[100]; // buffer to store words up until a token is found
  int token_len = 0,
      token_count = 0; // index functions to keep track of where to store words,
                       // and what char to replace inside of temp_token
  do {
    if (*str == ch || *str == '\0') {
      // once a token is found (or end of string), we store it in a new string
      char *token = malloc(token_len * sizeof(char) + 1);
      temp_token[token_len] = '\0'; // end buffer so it can be copied
      strcpy(token, temp_token); // make copy
      *(token_list + token_count) = token; // point towards newly found word
      token_count++;
      token_len = 0;
    } else {
      temp_token[token_len] = *str; // if no token has been found, keep storing chars
      token_len++;
    }
    str++;
  } while (token_count <= ch_rep); // keep going until we find all tokens (+ end of string)
  return token_list;
}

int main() {
  char *str = "Hello one two three four five! dd?";
  char tkn = ' ';
  char **tokenized = tokenize(str, tkn);
  int rep = repetitions(str, tkn);
  for (int i = 0; i < rep + 1; i++) {
    printf("%s\n", *(tokenized + i));
  }
  for (int i = 0; i < rep + 1; i++) {
    free(*(tokenized + i));
  }
  free(tokenized);
}
