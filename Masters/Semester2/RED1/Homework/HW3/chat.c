#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define MSG_SIZE 80
#define SHM_KEY 111

// shared memory will use this structure to know where to read and when to read.
// flags indicate if there is a new message which has not been read by the other user
// message simply contains the message written by the respective user
struct chat
{
  char flag_maria;
  char msg_maria[MSG_SIZE];
  char flag_jose;
  char msg_jose[MSG_SIZE];
};

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    fprintf(stderr, "Invalid argument count. Usage: %s <jose|maria>\n", argv[0]);
    exit(1);
  }

  // we will only use 2 specific users
  int is_maria = strcmp(argv[1], "maria") == 0;
  int is_jose = strcmp(argv[1], "jose") == 0;

  if (!is_maria && !is_jose)
  {
    fprintf(stderr, "Invalid users.\n");
    exit(1);
  }

  // create a shared memory space of the size of the struct 
  int shmid = shmget(SHM_KEY, sizeof(struct chat), IPC_CREAT | 0666);
  if (shmid < 0)
  {
    perror("shmget");
    exit(1);
  }

  // attach to the shared memory
  struct chat *data = (struct chat *)shmat(shmid, NULL, 0);
  if (data == (void *)-1)
  {
    perror("shmat");
    exit(1);
  }

  // set flags to zero, as none have written. This may cause some messages being
  // lost if a message is sent before the other person connects to the shared
  // memory
  data->flag_maria = 0;
  data->flag_jose = 0;

  int oldflags;
  if ((oldflags = fcntl(0, F_GETFL, 0)) < 0)
  {
    _exit(1);
  }

  if (fcntl(0, F_SETFL, O_NONBLOCK) < 0)
  {
    _exit(1);
  }

  char buffer[MSG_SIZE];

  while (1)
  {
    // read message from stdin
    if (fgets(buffer, MSG_SIZE, stdin) != NULL)
    {
      if (is_maria)
      {
        if (data->flag_maria == 0)
        {
          strncpy(data->msg_maria, buffer, MSG_SIZE);
          data->flag_maria = 1;
        }
      }
      else
      {
        if (data->flag_jose == 0)
        {
          strncpy(data->msg_jose, buffer, MSG_SIZE);
          data->flag_jose = 1;
        }
      }
    }

    // reads from shared memory and prints differ depending on who owns the process
    if (is_maria)
    {
      if (data->flag_jose == 1)
      {
        printf("Jose: %s", data->msg_jose);
        data->flag_jose = 0;
      }
    }
    else
    {
      if (data->flag_maria == 1)
      {
        printf("Maria: %s", data->msg_maria);
        data->flag_maria = 0;
      }
    }
    // 100 ms delay between checking
    usleep(100000);
  }
  return 0;
}