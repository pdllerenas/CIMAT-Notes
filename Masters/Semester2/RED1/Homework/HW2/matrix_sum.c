#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>

double *sum_3d_matrix(double ***pmatrix, int M, int N, int K, int P)
{
  int shmid = shmget(IPC_PRIVATE, M * N * sizeof(double), IPC_CREAT | 0666);
  if (shmid < 0)
  {
    perror("shmget failed");
    exit(1);
  }

  double *result = (double *)shmat(shmid, NULL, 0);
  if (result == (void *)-1)
  {
    perror("shmat failed");
    exit(1);
  }

  for (int proc = 0; proc < P; proc++)
  {
    pid_t pid = fork();

    if (pid == 0) // child
    {
      int start = (proc * M) / P;
      int end = ((proc + 1) * M) / P;

      for (int i = start; i < end; i++)
      {
        for (int j = 0; j < N; j++)
        {
          double sum = 0;
          for (int k = 0; k < K; k++)
            sum += pmatrix[i][j][k];

          result[i * N + j] = sum;
        }
      }

      shmdt(result);
      exit(0);
    }
  }

  for (int i = 0; i < P; i++)
    wait(NULL);

  shmctl(shmid, IPC_RMID, NULL); // mark for deletion

  return result;
}


int main(int argc, char *argv[])
{
  if (argc != 6)
  {
    fprintf(stderr, "Invalid input. Usage: %s <N> <M> <K> <P> <seed>\n", argv[0]);
    exit(1);
  }
  int N = atoi(argv[1]);
  int M = atoi(argv[2]);
  int K = atoi(argv[3]);
  int P = atoi(argv[4]);
  int seed = atoi(argv[5]);

  if (N <= 0 || M <= 0 || K <= 0 || P <= 0)
  {
    fprintf(stderr, "Invalid size\n");
    exit(1);
  }

  double ***matrix = malloc(M * sizeof(double **));
  for (int i = 0; i < M; i++)
  {
    matrix[i] = malloc(N * sizeof(double *));
    for (int j = 0; j < N; j++)
    {
      matrix[i][j] = malloc(K * sizeof(double));
    }
  }
  srand(seed);
  for (int i = 0; i < M; i++)
    for (int j = 0; j < N; j++)
      for (int k = 0; k < K; k++)
        matrix[i][j][k] = (rand() % 100) / 100.0;

  clock_t start_t, end_t;
  double total_t;

  start_t = clock();
  double *result = sum_3d_matrix(matrix, M, N, K, P);
  end_t = clock();

  total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
  printf("Total time taken by CPU: %f seconds\n", total_t);

  // free matrix
  for (int i = 0; i < M; i++)
  {
    for (int j = 0; j < N; j++)
      free(matrix[i][j]);
    free(matrix[i]);
  }
  free(matrix);

  return 0;
}