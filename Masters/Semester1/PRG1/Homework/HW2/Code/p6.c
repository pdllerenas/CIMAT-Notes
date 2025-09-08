#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// #define MAX_ROW_SIZE 100
// #define MAX_COL_SIZE 20

void print(int **arr, int N, int *dim) {
  for (int i = 0; i < N; i++) {
    int curr_dim = *(dim + i);
    for (int j = 0; j < curr_dim; j++) {
      printf("%d ", *(*(arr + i) + j));
    }
    printf("\n");
  }
}

void print_1d(int *arr, int N) {
  for (int i = 0; i < N; i++) {
    printf("%d ", *(arr + i));
  }
  printf("\n");
}

int arr_sum(int *arr, int dim) {
  int result = 0;
  for (int i = 0; i < dim; i++) {
    result += *(arr + i);
  }
  return result;
}

int *merge(int **arr, int N, int *dim) {
  int new_dim = arr_sum(dim, N); // new dimension of merged arrays.
  int minimum = INT_MAX;         // where we keep track of smallest val.

  // array where we store sorted values
  int *merged = malloc(new_dim * sizeof(int));

  // to keep track of where we are in *(arr + k),
  // init at 0, then keep adding as we traverse *(arr + k).
  int *dim_idx = calloc(N, sizeof(int));
  int row_selected; // keep track of what row had the min value.
  for (int i = 0; i < new_dim; i++) {
    for (int j = 0; j < N; j++) {
      // we check if dim_idx is still within the dimension of *(arr + k),
      // and we also check if that value can be our new minimum.
      if (*(dim_idx + j) < *(dim + j) &&
          *(*(arr + j) + *(dim_idx + j)) < minimum) {
        minimum = *(*(arr + j) + *(dim_idx + j));
        row_selected = j; // keep track that min was found in row j.
      }
    }
    // afer first minimum was found, place it in result array.
    *(merged + i) = minimum;
    minimum = INT_MAX;              // reset minimum for next search.
    *(dim_idx + row_selected) += 1; // update *(row + k) current index, so we do
                                    // not repeat an already chosen value.
  }
  free(dim_idx);
  return merged;
}

int main() {
  int MAX_ROW_SIZE, MAX_COL_SIZE;
  printf("Enter max row size and max col size. Format: 'N,M'\n");
  scanf("%d,%d", &MAX_ROW_SIZE, &MAX_COL_SIZE);
  srand(time(NULL));
  float sTime = (float)clock() / CLOCKS_PER_SEC;
  unsigned int rand_dim = 1 + rand() % MAX_ROW_SIZE; // random number of rows.
  int **arr = malloc(rand_dim * sizeof(*arr));
  int *dim = malloc(rand_dim * sizeof(int)); // dimensions of each row.
  for (int i = 0; i < rand_dim; i++) {
    int rand_arr_size = 1 + rand() % MAX_COL_SIZE;  // random dimension for row.
    *(dim + i) = rand_arr_size;                     // point to array size.
    int *row = malloc(rand_arr_size * sizeof(int)); // make space for row.
    int max_val = 10;
    for (int j = 0; j < rand_arr_size; j++) {
      if (j > 0) {
        *(row + j) =
            *(row + j - 1) +
            rand() % max_val; // next value is in range (prev, prev + 9).
      } else {
        *row = rand() % max_val; // first value is in range (0, 9).
      }
      *(arr + i) = row; // points to row.
    }
  }
  float eTime = (float)clock() / CLOCKS_PER_SEC;
  printf("-----------------------------------------------------------------\n");
  printf("Randomly generated matrix: (execution time: %f ms)\n",
         1000 * (eTime - sTime));
  printf("-----------------------------------------------------------------\n");
  print(arr, rand_dim, dim);

  sTime = (float)clock() / CLOCKS_PER_SEC;
  int *merged = merge(arr, rand_dim, dim);
  eTime = (float)clock() / CLOCKS_PER_SEC;
  printf("-----------------------------------------------------------------\n");
  printf("Sorted merged array: (execution time: %f ms)\n",
         1000 * (eTime - sTime));
  printf("-----------------------------------------------------------------\n");
  print_1d(merged, arr_sum(dim, rand_dim));

  for (int i = 0; i < rand_dim; i++) {
    free(*(arr + i));
  }
  free(dim);
  free(arr);
  free(merged);

  return 0;
}
