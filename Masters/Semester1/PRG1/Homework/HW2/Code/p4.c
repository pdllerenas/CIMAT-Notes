#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int count_new_line(FILE *file) {
  int c, count = 0;
  while ((c = fgetc(file)) != EOF) {
    if (c == '\n') {
      count++;
    }
  }
  return count;
}

char **create_array(FILE *file, int size) {
  int c, name_length = 0, name_count = 0;
  char **arr;
  arr = malloc((size + 1) * sizeof(*arr));
  rewind(file);

  char buffer[100];

  while ((c = fgetc(file)) != EOF) {
    if (c == '\n') {
      buffer[name_length] = '\0';
      char *name = malloc(name_length * sizeof(char));
      strcpy(name, buffer);
      *(arr + name_count) = name;
      name_count++;
      name_length = 0;
    } else {
      buffer[name_length] = c;
      name_length++;
    }
  }
  fclose(file);
  return arr;
}

char **order(char **arr, int size) {
  char *current, *temp;
  int str_diff;
  for (int i = 0; i < size; i++) {
    current = *(arr + i);
    for (int j = i - 1; j >= 0; j--) {
      printf("b4: %s , %s\n", *(arr + j), current);
      str_diff = strcmp(*(arr + j), current);
      // str_diff is positive if rhs precedes lhs, that is,
      // current precedes *(arr+j)
      if (str_diff <= 0)
        break;
      if (str_diff > 0) {
        temp = current;
        current = *(arr + j);
        *(arr + j) = temp;
        printf("af: %s , %s\n", *(arr + j), current);
      }
    }
  }

  return arr;
}

void print(char **arr, int n) {
  for (int i = 0; i < n; i++) {
    printf("%s\n", *(arr + i));
  }
}

int main() {
  FILE *file;
  file = fopen("names.csv", "r");

  int size = count_new_line(file);

  char **names = create_array(file, size);
  names = order(names, size);
  // print(names, 12);
  free(names);
  return 0;
}
