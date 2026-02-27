#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

#include "convolution.h"

/*
calculates the (x,y) coordinate of the convolution of (k, matrix)
 */

static int convolution(const Kernel *k, unsigned char **matrix, const int y, const int x, const int w, const int h)
{
  int sum = 0;
  for (int i = 0; i < k->dimension; i++)
  {
    for (int j = 0; j < k->dimension; j++)
    {
      // matrix offset
      int m_x = x - k->dimension / 2 + j;
      int m_y = y - k->dimension / 2 + i;

      // check if out of matrix bounds
      if (m_x < 0 || m_y < 0 || m_x >= w || m_y >= h)
      {
        sum += k->data[k->dimension * i + j] * 0;
        continue;
      }
      // if inside matrix, multiply
      sum += k->data[k->dimension * i + j] * matrix[m_y][m_x];
    }
  }
  // divide by kernel divisor. e.g. 3x3 gaussian has divisor 16
  return sum / k->divisor;
}

/*
  applies the mean kernel of dimensions k_dim * k_dim to the given input image
*/

unsigned char **apply_mean(unsigned char **input, int rows, int cols,
                           Kernel *(*k)(int), int k_dim, int p)
{
  Kernel *kernel = k(k_dim);

  size_t total_pixels = rows * cols;
  size_t size_img = total_pixels * sizeof(unsigned char);

  /* -------- Shared memory for input -------- */
  int shmid_img = shmget(IPC_PRIVATE, size_img, 0666 | IPC_CREAT);
  if (shmid_img < 0)
  {
    perror("shmget input failed");
    exit(1);
  }

  unsigned char *shared_input = shmat(shmid_img, NULL, 0);
  if (shared_input == (void *)-1)
  {
    perror("shmat input failed");
    exit(1);
  }

  /* Copy input into shared memory */
  for (int i = 0; i < rows; i++)
    memcpy(shared_input + i * cols, input[i], cols);

  /* -------- Shared memory for output -------- */
  int shmid_out = shmget(IPC_PRIVATE, size_img, 0666 | IPC_CREAT);
  if (shmid_out < 0)
  {
    perror("shmget output failed");
    exit(1);
  }

  unsigned char *shared_out = shmat(shmid_out, NULL, 0);
  if (shared_out == (void *)-1)
  {
    perror("shmat output failed");
    exit(1);
  }

  memset(shared_out, 0, size_img);

  for (int proc = 0; proc < p; proc++)
  {
    pid_t pid = fork();

    if (pid == 0) // child
    {
      int start = (proc * rows) / p;
      int end = ((proc + 1) * rows) / p;

      for (int y = start; y < end; y++)
        for (int x = 0; x < cols; x++)
          shared_out[y * cols + x] = convolution(kernel, input, y, x, rows, cols);

      shmdt(shared_input);
      shmdt(shared_out);
      exit(0);
    }
  }

  for (int i = 0; i < p; i++)
    wait(NULL);

  unsigned char **output = malloc(rows * sizeof(unsigned char *));
  unsigned char *buffer = malloc(total_pixels * sizeof(unsigned char));

  for (int i = 0; i < rows; i++)
    output[i] = buffer + i * cols;

  for (int i = 0; i < rows; i++)
    memcpy(output[i], shared_out + i * cols, cols);

  shmdt(shared_input);
  shmdt(shared_out);
  shmctl(shmid_img, IPC_RMID, NULL);
  shmctl(shmid_out, IPC_RMID, NULL);
  free_kernel(kernel);

  return output;
}

/*

calculates the median of a submatrix of input of dimensions
median_dim, assuming the subimatrix is centered at (col = y, row = x)

 */
static unsigned char sub_matrix_median(unsigned char **input, int rows, int cols, int y, int x, int median_dim)
{
  int arr_dim = median_dim * median_dim;

  // helper array to sort and then find median
  unsigned char *sorted_arr = malloc(arr_dim * sizeof(unsigned char));
  if (!sorted_arr)
  {
    perror("Could not allocate memory.\n");
    exit(1);
  }

  for (int i = 0; i < median_dim; i++)
  {
    for (int j = 0; j < median_dim; j++)
    {
      // check out of matrix bounds values,
      // we assume these are black pixels
      if (y + i - 1 < 0 || x + j - 1 < 0 || y + i - 1 >= rows ||
          x + j - 1 >= cols)
      {
        sorted_arr[i * median_dim + j] = 0;
        continue;
      }
      sorted_arr[i * median_dim + j] = input[y + i - 1][x + j - 1];
    }
  }
  // sort the array by comparing unsigned chars
  qsort(sorted_arr, arr_dim, sizeof(unsigned char), compare_uchar);
  int median = (arr_dim % 2 == 1)
                   ? (sorted_arr[arr_dim / 2])
                   : (sorted_arr[(arr_dim - 1) / 2 + (arr_dim) / 2]);
  free(sorted_arr);
  return median;
}

/*

applies the median filter of size median_dim * median_dim to the input image

 */

unsigned char **median_filter_nxn(unsigned char **input, int rows, int cols, int median_dim, int p)
{
  size_t total_pixels = rows * cols;
  size_t size_img = total_pixels * sizeof(unsigned char);

  int shmid_out = shmget(IPC_PRIVATE, size_img, IPC_CREAT | 0666);
  if (shmid_out < 0)
  {
    perror("shmget failed");
    exit(1);
  }

  unsigned char *shared_out = (unsigned char *)shmat(shmid_out, NULL, 0);
  if (shared_out == (void *)-1)
  {
    perror("shmat failed");
    exit(1);
  }

  for (int proc = 0; proc < p; proc++)
  {
    pid_t pid = fork();

    if (pid == 0) // child
    {
      int start = (proc * rows) / p;
      int end = ((proc + 1) * rows) / p;

      for (int y = start; y < end; y++)
        for (int x = 0; x < cols; x++)
          shared_out[y * cols + x] = sub_matrix_median(input, rows, cols, y, x, median_dim);

      shmdt(shared_out);
      exit(0);
    }
  }

  for (int i = 0; i < p; i++)
    wait(NULL);

  unsigned char **output = malloc(rows * sizeof(unsigned char *));
  unsigned char *buffer = malloc(size_img);

  if (!output || !buffer)
  {
    perror("Allocation failed");
    exit(1);
  }

  for (int i = 0; i < rows; i++)
    output[i] = buffer + i * cols;

  for (int i = 0; i < rows; i++)
    memcpy(output[i], shared_out + i * cols, cols);

  shmdt(shared_out);
  shmctl(shmid_out, IPC_RMID, NULL);

  return output;
}
