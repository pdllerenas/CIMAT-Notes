#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  pid_t pid;
  pid = fork();
  int a = 0;

  switch (pid) {
  case -1:
    perror("fork");
    exit(EXIT_FAILURE);
  case 0:
    a = 10;
    puts("Child exiting.");
    fflush(stdout);
    _exit(EXIT_SUCCESS);
  }
  pid = wait(NULL);
  printf("Parent pid = %d\n", getpid());
  printf("Child pid = %d\n", pid);
  sleep(4);
  printf("aaa\n");
  return 0;
}
