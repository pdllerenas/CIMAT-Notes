#include <limits.h> // used limits.h to have access to INT_MIN for compatibility with lower bit machines.
#include <stdio.h>

/*

pretty prints the values of the array a

 */
void print_array(int *a, int n, int m) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      printf("%d ", *((a + i * n) + j));
    }
    printf("\n");
  }
}

/*

traverses the array a and stores the max of row i into row_max[i], as well as
storing the max of col j into col_max[j]. This is O(N*M) time complexity with
O(N+M) extra space (arrays row_max and col_max)
Note: if we do not consider the return values as extra space, then we only need
O(1) (curr pointer) extra space.

 */

void max_col_row(int *a, int n, int m, int *row_max, int *col_max) {
  int *curr = a;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      if (*curr > row_max[i])
        row_max[i] = *curr;
      if (*curr > col_max[j])
        col_max[j] = *curr;
      curr++;
    }
  }
}

/*

traverses the array a and returns the number of zeros found times sizeof(int).
That is, the number of bytes occupied by the number 0 in the array a.

 */

int zeros(int *a, int n, int m) {
  int zeros = 0, *p;
  for (p = a; p < a + n * m; p++) {
    if (*p == 0)
      zeros += sizeof(int);
  }
  return zeros;
}

int main() {
  unsigned int N, M;
  scanf("%ud, %ud", &N, &M);
  int row_max[N]; // array to store the max of each row
  int col_max[M]; // array to store the max of each column
  for (int k = 0; k < N; k++) {
    row_max[k] =
        INT_MIN; // initialize row_max to negative inf (minimum integer)
  }
  for (int k = 0; k < M; k++) {
    col_max[k] =
        INT_MIN; // initialize col_max to negative inf (minimum integer)
  }

  printf("Enter row and columns formatted as 'N,M':\n");
  printf("N = %d, M = %d\n", N, M);

  // array which will be used to store the values entered by the user

  int a[N][M];
  int *p = &a[0][0], // pointer p will be used to initialize array a.
      *END = &a[0][0] + N * M;
  // END is the last + sizeof(int) memory slot to
  // stop the initialization process

  printf("Enter cell values.\n");
  while (p < END) {
    scanf("%d", p++); // assign scanned value to whatever p is pointing to, then
                      // increment it by 1 (that is, sizeof(int))
  }
  max_col_row((int *)a, N, M, row_max,
              col_max); // calculate row and col max, which will be stored in
                        // row_max and col_max.

  print_array(row_max, 1, N); // print row_max values
  print_array(col_max, 1, M); // print col_max values

  printf("0's used %d bytes of array a\n",
         zeros((int *)a, N, M)); // print number of bytes used by the zeros
                                 // entered into the array a
}
