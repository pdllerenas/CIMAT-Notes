#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char *title;
  int year;
} Article;

typedef struct {
  char *name; // full name
  char *affiliation;
  Article *info;
  char *country;
} Author;

int compare_author_name(const void *a, const void *b) {
  const Author *authorA = *(const Author **)a;
  const Author *authorB = *(const Author **)b;
  return strcmp(authorA->name, authorB->name);
}

int compare_article_year(const void *a, const void *b) {
  const Author *authorA = *(const Author **)a;
  const Author *authorB = *(const Author **)b;
  return (authorA->info->year > authorB->info->year) -
         (authorA->info->year < authorB->info->year);
}

void order_authors(Author **authors, int n,
                   int (*comp)(const void *, const void *)) {
  qsort(authors, n, sizeof(Author *), comp);
}

int get_cols(char *line) {
  const char *token;
  int cols;
  for (token = strtok(line, ","); token && *token;
       token = strtok(NULL, ",\n")) {
    cols++;
  }
  return cols;
}

/*

parses the given csv, assuming that the structure is that of

string string string string int, all separated with a comma

 */
Author **csv_reader(const char *filename, int *rows) {
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    perror(filename);
    return NULL;
  }
  int size = 8;
  Author **authors = malloc(size * sizeof(Author *));

  char line[1024];
  int cols;
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

    // author name
    tokens = strtok(line, ",");
    auth_row->name = strdup(tokens);

    // article title
    tokens = strtok(NULL, ",\n");
    Article *article = malloc(sizeof(Article));
    article->title = strdup(tokens);

    // author affiliation
    tokens = strtok(NULL, ",\n");
    auth_row->affiliation = strdup(tokens);

    // author country
    tokens = strtok(NULL, ",\n");
    auth_row->country = strdup(tokens);

    // article year
    tokens = strtok(NULL, ",\n");
    article->year = atoi(tokens);

    // store article info in auth_row struct
    auth_row->info = article;

    // use auth_row pointer for general array
    authors[*rows] = auth_row;
    (*rows)++;
  }
  fclose(fp);
  return authors;
}

void print_author(const Author *author) {
  printf("name: %s\n", author->name);
  printf("affiliation: %s\n", author->affiliation);
  printf("article: %s\n", author->info->title);
  printf("year: %d\n", author->info->year);
  printf("country: %s\n", author->country);
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(
        stderr,
        "Invalid argument count. Usage: %s <file-input> <sort [name | year]>\n",
        argv[0]);
    exit(1);
  }
  int rows;
  Author **authors = csv_reader(argv[1], &rows);

  if (strcmp(argv[2], "name") == 0) {
    order_authors(authors, rows, compare_author_name);
  } else if (strcmp(argv[2], "year") == 0) {
    order_authors(authors, rows, compare_article_year);
  } else {
    fprintf(stderr, "Invalid sorting argument.\n");
    exit(1);
  }

  for (int i = 0; i < rows; i++) {
    print_author(authors[i]);
    printf("\n");
  }
  // since each entry was copied using strdup, we must free every single struct
  // attribute (except year, since it is not a string duplication)
  for (int i = 0; i < rows; i++) {
    free(authors[i]->name);
    free(authors[i]->affiliation);
    free(authors[i]->country);
    free(authors[i]->info->title);
    free(authors[i]->info);
    free(authors[i]);
  }
  free(authors);
}
