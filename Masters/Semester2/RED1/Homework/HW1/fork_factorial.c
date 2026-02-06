#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/*
 * A static decomposition of the factorial function. This will divide the
 * product 1 * ... * N into p chunks (not necessaraly equal in size). Each chunk
 * product is relayed to the parent process, which awaits for the next finished
 * task to multiply the results. Note that this uses the associativity of the
 * product of natural numbers. This might not generalize to floating point
 * numbers, as (a * b) * c != a * (b * c).
 */
unsigned long factorial(unsigned int N, unsigned int p) {
  if (p < 1) {
    printf("Error: Invalid number of processes\n");
    return 0;
  }
  if (p > log2(N)) {
    p = floor(log2(N));
    printf("Setting process number to reasonable amount: %d...\n", p);
  }

  pid_t pid;
	int fd[2];

  int chunk = N / p;
  int rem = N % p;
  for (int i = 0; i < p; i++) {
    if ((pid = fork() == 0)) {
      int start = i * chunk + 1;
      int end = start + chunk - 1;

      if (i < rem) {
        start += i;
        end++;
      } else {
        start += rem;
      }

      unsigned long partial = 1;
      for (int k = start; k < end; k++) {
        partial *= k;
      }
			write(fd[1], &partial, sizeof(partial));
			close(fd[1]);
      exit(EXIT_SUCCESS);
    }
  }

	unsigned long partial;
  unsigned long result = 1;
  for (int i = 0; i < p; i++) {
		read(fd[0], &partial, sizeof(partial));
		close(fd[0]);
		result *= partial;
    wait(NULL);
  }
  return result;
}

int main() {
  unsigned int N, p;
  if (scanf("%d %d", &N, &p) != 2) {
    printf("Invalid input.\n");
    return 1;
  }
  unsigned long f = factorial(N, p);
  printf("%d! = %lu", N, f);
  return 0;
}
