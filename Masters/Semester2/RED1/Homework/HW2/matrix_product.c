#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>

double *matrix_multiply(double *A, double *B, int N, int P)
{
  // for shared memory for result
  int shmid = shmget(IPC_PRIVATE, N * N * sizeof(double),
                     IPC_CREAT | 0666);
  if (shmid < 0)
  {
    perror("shmget failed");
    exit(1);
  }

  // attach shared memory
  double *C = (double *)shmat(shmid, NULL, 0);
  if (C == (void *)-1)
  {
    perror("shmat failed");
    exit(1);
  }

  for (int proc = 0; proc < P; proc++)
  {
    pid_t pid = fork();

    if (pid == 0) // child
    {
      int start = (proc * N) / P;
      int end = ((proc + 1) * N) / P;

      for (int i = start; i < end; i++)
      {
        for (int j = 0; j < N; j++)
        {
          double sum = 0;
          for (int k = 0; k < N; k++)
            sum += A[i * N + k] * B[k * N + j];

          C[i * N + j] = sum;
        }
      }

      shmdt(C);
      exit(0);
    }
  }

  for (int i = 0; i < P; i++)
    wait(NULL);

  shmctl(shmid, IPC_RMID, NULL);

  return C;
}

int main(int argc, char *argv[])
{
  if (argc != 4)
  {
    fprintf(stderr, "Invalid input. Usage: %s <N> <P> <seed>\n", argv[0]);
    exit(1);
  }
  int N = atoi(argv[1]);
  int P = atoi(argv[2]);
  int seed = atoi(argv[3]);

  if (N <= 0 || P <= 0)
  {
    fprintf(stderr, "Invalid size\n");
    exit(1);
  }

  double *A = malloc(N * N * sizeof(double));
  double *B = malloc(N * N * sizeof(double));

  srand(seed);
  for (int i = 0; i < N; i++)
  {
    for (int j = 0; j < N; j++)
    {
      A[i * N + j] = (rand() % 100) / 100.0;
      B[i * N + j] = (rand() % 100) / 100.0;
    }
  }

  clock_t start_t, end_t;
  double total_t;

  start_t = clock();
  double *C = matrix_multiply(A, B, N, P);
  end_t = clock();

  total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
  printf("Total time taken by CPU: %f seconds\n", total_t);

  free(A);
  free(B);

  return 0;
}