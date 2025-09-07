#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print(char **arr, int n) {
  for (int i = 0; i < n; i++) {
    printf("%s\n", *(arr + i));
  }
}

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
      buffer[name_length] = '\0'; // end name with '\0'

      // name_length + 1 to include '\0'
      char *name = malloc((name_length + 1) * sizeof(char));
      strcpy(name, &buffer[0]); // copy stored name in buffer into name pointer
      *(arr + name_count) = name; // arr + name_count now points to name
      name_count++;               // one more name added to list
      name_length = 0;            // reset name_length for next name in csv
    } else {
      buffer[name_length] = c; // store char to buffer
      name_length++;
    }
  }
  fclose(file);
  return arr;
}

char **order(char **arr, int size) {
  char *temp, *compare;
  int str_diff;
  for (int i = 0; i < size; i++) {
    compare = *(arr + i);
    for (int j = i - 1; j >= 0; j--) {
      str_diff = strcmp(*(arr + j), compare);
      // str_diff is positive if rhs precedes lhs, that is,
      // current precedes *(arr+j)

      if (str_diff <= 0) { // if in order, skip to next i
        break;
      }

      if (str_diff > 0) { // if out of order, swap j and j + 1
        // note that we swap j and j+1 and not j and i, because if we swapped i
        // and j in the previous iteration, we would now be swapping the
        // incorrect pointers.
        temp = *(arr + j + 1);
        *(arr + j + 1) = *(arr + j);
        *(arr + j) = temp;
      }
    }
  }

  return arr;
}

int main() {
  FILE *file;
  file = fopen("names.csv", "r");

  int size = count_new_line(
      file); // count new lines to know how many names are in the csv

  char **names = create_array(file, size); // create array using file
  order(names, size);                      // sort array in place
  print(names, size);                      // print ordered names
  for (int i = 0; i < size; i++) {
    free(names[i]);
  }
  free(names);
  return 0;
}
