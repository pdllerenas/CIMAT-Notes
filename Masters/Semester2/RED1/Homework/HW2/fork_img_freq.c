#include "pgm_io.h"
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>

#define SHM_SIZE 1024

unsigned int *image_freq(unsigned char *image, int rows, int cols, int threads)
{
  size_t total_pixels = rows * cols;

  // Shared image
  size_t size_img = total_pixels * sizeof(unsigned char);
  int shmid_img = shmget(IPC_PRIVATE, size_img, 0666 | IPC_CREAT);
  if (shmid_img < 0)
  {
    perror("shmget failed");
    exit(1);
  }

  unsigned char *shared_image = shmat(shmid_img, NULL, 0);
  if (shared_image == (void *)-1)
  {
    perror("shmat failed");
    exit(1);
  }

  memcpy(shared_image, image, size_img);

  // Shared frequency matrix (threads x 256) 
  size_t size_freq = threads * 256 * sizeof(unsigned int);
  int shmid_freq = shmget(IPC_PRIVATE, size_freq, 0666 | IPC_CREAT);
  if (shmid_freq < 0)
  {
    perror("shmget failed");
    exit(1);
  }

  unsigned int *shared_freq = shmat(shmid_freq, NULL, 0);
  if (shared_freq == (void *)-1)
  {
    perror("shmat failed");
    exit(1);
  }

  memset(shared_freq, 0, size_freq);

  // mark for deletion
  shmctl(shmid_img, IPC_RMID, NULL);
  shmctl(shmid_freq, IPC_RMID, NULL);

  int chunk = total_pixels / threads;

  for (int t = 0; t < threads; t++)
  {
    pid_t pid = fork();
    if (pid < 0)
    {
      perror("fork failed");
      exit(1);
    }

    if (pid == 0)
    {
      int start = t * chunk;
      int end = (t == threads - 1) ? total_pixels : (t + 1) * chunk;

      unsigned int *my_freq = shared_freq + t * 256;

      for (int i = start; i < end; i++)
        my_freq[shared_image[i]]++;

      shmdt(shared_image);
      shmdt(shared_freq);
      exit(0);
    }
  }

  // wait for children
  for (int t = 0; t < threads; t++)
    wait(NULL);

  unsigned int *final_freq = malloc(256 * sizeof(unsigned int));
  if (!final_freq)
  {
    perror("malloc");
    exit(1);
  }
  memset(final_freq, 0, 256 * sizeof(unsigned int));

  for (int t = 0; t < threads; t++)
  {
    unsigned int *thread_freq = shared_freq + t * 256;

    for (int i = 0; i < 256; i++)
      final_freq[i] += thread_freq[i];
  }

  shmdt(shared_image);
  shmdt(shared_freq);

  return final_freq;
}

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    fprintf(stderr, "Invalid argument count\nUsage: %s <pgm> <thread count>\n", argv[0]);
    exit(1);
  }
  int threads = atoi(argv[2]);
  if (threads <= 0)
  {
    fprintf(stderr, "Invalid thread count\n");
    exit(1);
  }
  int rows, cols;
  unsigned char **image = pgmRead(argv[1], &rows, &cols);
  if (image == NULL)
  {
    fprintf(stderr, "Error pgmRead\n");
    exit(1);
  }

  unsigned char *contiguous_image = (unsigned char *)malloc(rows * cols * sizeof(unsigned char));

  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      contiguous_image[i * cols + j] = image[i][j];
    }
  }

  clock_t start_t, end_t;
  double total_t;

  start_t = clock();
  unsigned int *freq = image_freq(contiguous_image, rows, cols, threads);
  if (!freq)
  {
    fprintf(stderr, "Unable to read frequencies.\n");
    exit(1);
  }
  end_t = clock();

  total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
  printf("Total time taken by CPU: %f seconds\n", total_t);

  shmdt(freq);
  free(image[0]);
  free(image);
}