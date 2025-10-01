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

void free_authors(Author **authors, int rows) {
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
/*

assumes both a and b can be converted to an Author pointer, and that their name
property can be compared meaningfully

 */
int compare_author_name(const void *a, const void *b) {
  const Author *authorA = *(const Author **)a;
  const Author *authorB = *(const Author **)b;
  return strcmp(authorA->name, authorB->name);
}

/*

assumes both a and b can be converted to an Author pointer, and that their year
property can be compared meaningfully

 */
int compare_article_year(const void *a, const void *b) {
  const Author *authorA = *(const Author **)a;
  const Author *authorB = *(const Author **)b;
  return (authorA->info->year > authorB->info->year) -
         (authorA->info->year < authorB->info->year);
}

/*

uses qsort to sort w.r.t year or name, depending on comp (comparing function)
passed

 */
void order_authors(Author **authors, int n,
                   int (*comp)(const void *, const void *)) {
  qsort(authors, n, sizeof(Author *), comp);
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
  *rows = 0;
  fgets(line, 1024, fp);

  char *temp;
  char *tokens;

  while (fgets(line, 1024, fp)) {
    Author *auth_row = malloc(sizeof(Author));
    if (!auth_row) {
      perror("Unable to allocate memory.\n");
      free_authors(authors, *rows);
      fclose(fp);
      return NULL;
    }
    // realloc if necessary
    if (*rows == size) {
      size *= 2;
      Author **temp = realloc(authors, size * sizeof(Author *));
      if (!temp) {
        perror("Failed to reallocate memory.\n");
        free_authors(authors, *rows);
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

void fprint_author(FILE *f, const Author *author) {
  fprintf(f, "%s,", author->name);
  fprintf(f, "%s,", author->info->title);
  fprintf(f, "%s,", author->affiliation);
  fprintf(f, "%s,", author->country);
  fprintf(f, "%d", author->info->year);
}

int main(int argc, char *argv[]) {
  if (argc != 4) {
    fprintf(stderr,
            "Invalid argument count. Usage: %s <file-input> <sort [name | "
            "year]> <file-out>\n",
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

  FILE *f = fopen(argv[3], "w");
  if (!f) {
    perror(argv[3]);
    exit(1);
  }

  fprintf(f, "Name,Article,Affiliation,Country/Region,year\n");
  for (int i = 0; i < rows; i++) {
    fprint_author(f, authors[i]);
    fprintf(f, "\n");
  }

  free_authors(authors, rows);
  fclose(f);

  // since each entry was copied using strdup, we must free every single struct
  // attribute (except year, since it is not a string duplication)
}
