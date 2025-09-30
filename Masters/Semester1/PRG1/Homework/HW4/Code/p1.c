#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char *title;
  int year;
} Article;

typedef struct {
  char *name; // full name
  char *afiliation;
  Article *info;
  char *country;
} Author;


int get_cols(char *line) {
  const char *token;
  int cols;
  for (token = strtok(line, ","); token && *token;
       token = strtok(NULL, ",\n")) {
    cols++;
  }
  return cols;
}

Author **csv_reader(const char *filename, int *rows) {
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    perror(filename);
    return NULL;
  }
  int size = 8;
  Author **authors = malloc(size * sizeof(Author *));

  char line[1024];
  int cols, row = 0;
  *rows = 0;
  fgets(line, 1024, fp);
  cols = get_cols(line);

  char *temp;
  char *tokens;
  while (fgets(line, 1024, fp)) {
    Author *auth_row = malloc(sizeof(Author));
    // realloc if necessary
    if (*rows == size) {
      size *= 2;
      Author **temp = realloc(authors, size * sizeof(Author *));
      if (!temp) {
        perror("Failed to reallocate memory.\n");
        for (int i = 0; i < size / 2; i++) {
          free(authors[i]->info);
          free(authors[i]);
        }
        free(authors);
        fclose(fp);
        return NULL;
      }
      authors = temp;
    }
    // attribute handling
    tokens = strtok(line, ",");
    auth_row->name = strdup(tokens);

    tokens = strtok(NULL, ",\n");
    Article *article = malloc(sizeof(Article));
    article->title = strdup(tokens);
    article->year = 2020;
    auth_row->info = article;

    tokens = strtok(NULL, ",\n");
    auth_row->afiliation = strdup(tokens);

    tokens = strtok(NULL, ",\n");
    auth_row->country = strdup(tokens);

    authors[row] = auth_row;
    (*rows)++;
    row++;
  }
  fclose(fp);
  return authors;
}

void print_author(const Author *author) {
  printf("name: %s\n", author->name);
  printf("afiliation: %s\n", author->afiliation);
  printf("article: %s\n", author->info->title);
  printf("country: %s\n", author->country);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Invalid argument count. Usage: %s <file-input>\n",
            argv[0]);
  }
  int rows;
  Author **authors = csv_reader(argv[1], &rows);

  for (int i = 0; i < rows; i++) {
    print_author(authors[i]);
    printf("\n");
  }
  for (int i = 0; i < rows; i++) {
    free(authors[i]->info);
    free(authors[i]);
  }
  free(authors);
}
