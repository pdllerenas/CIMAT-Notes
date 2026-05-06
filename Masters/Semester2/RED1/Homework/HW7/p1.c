#include <stdio.h>
#include <signal.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

// 1200 byte strings
#define SHM_SIZE 1200

static int s = 0;

void handler(int signum)
{
    if (signum == SIGINT)
    {
        printf("Exitint program.\n");
        s = -1;
    }
    else if (signum == SIGUSR1)
    {
        printf("Data available in shared memory.\n");
        s = 0;
    }
    else if (signum == SIGUSR2)
    {
        printf("Data read by child.\n");
        s = 0;
    }
    else
    {
        printf("Unhandled exception.\n");
        s = -2;
    }
}

int CopyFileToSHM(const char *filename, char *shm, pid_t pid)
{
    struct sigaction sa_new;
    struct sigaction sa_old;

    sigset_t sigSetNew, sigSetOld, suspendSet;

    sa_new.sa_handler = handler;
    sigemptyset(&sa_new.sa_mask);
    sa_new.sa_flags = 0;

    sigaction(SIGUSR2, &sa_new, &sa_old);
    sigemptyset(&suspendSet);
    sigfillset(&suspendSet);
    sigdelset(&suspendSet, SIGUSR2);

    FILE *fptr = fopen(filename, "r");
    if (fptr == NULL)
    {
        perror("fopen failed");
        return 1;
    }

    size_t bytes_read = 0;

    while ((bytes_read = fread(shm + bytes_read, 1, SHM_SIZE, fptr)) > 0)
    {
        int status;
        kill(pid, SIGUSR1);
        sigsuspend(&suspendSet);
        if (s != 0) {
            break;
        }
    }
    kill(pid, SIGINT);

    fclose(fptr);
    return 0;
}

void CopySHMToFile(const char *filename, char *shm, pid_t pid)
{
    struct sigaction sa_new;
    struct sigaction sa_old;

    sigset_t sigSetNew, sigSetOld, suspendSet;

    sa_new.sa_handler = handler;
    sigemptyset(&sa_new.sa_mask);
    sa_new.sa_flags = 0;

    sigaction(SIGUSR1, &sa_new, &sa_old);
    sigemptyset(&suspendSet);
    sigfillset(&suspendSet);
    sigdelset(&suspendSet, SIGUSR1);
    sigdelset(&suspendSet, SIGINT);

    FILE *fptr = fopen(filename, "w");
    size_t bytes_read = 0;
    while (1)
    {
        if ((bytes_read = fwrite(shm + bytes_read, SHM_SIZE, 1, fptr)) <= 0)
        {
            return;
        }
        kill(pid, SIGUSR2);
        sigsuspend(&suspendSet);
        if (s != 0) {
            break;
        }
    }

    fclose(fptr);
}

void NonsenseWriter(const char *filename)
{
    FILE *fptr = fopen(filename, "w");
    for (int i = 0; i < 80 * SHM_SIZE; i++)
    {
        fputc('a' + (rand() % 10), fptr);
    }
}

int main(void)
{
    // generate key
    key_t key = ftok("shmfile", 65);
    pid_t parentid = getpid();

    // get shared memory id, create if non existent
    int shmid = shmget(key, SHM_SIZE * 80, 0666 | IPC_CREAT);

    // attach to shared memory
    char *str = (char *)shmat(shmid, (void *)0, 0);

    if (str == (char *)(-1))
    {
        perror("shmat failed");
        return 1;
    }

    pid_t pid = fork();

    // child will read from shared memory
    if (pid == 0)
    {
        // signal(SIGUSR1, HandleItemsWritten);
        // signal(SIGINT, HandleInterrupt);
        pause();
        printf("Data read from shared memory: %s\n", str);
        CopySHMToFile("out.txt", str, parentid);
    }
    else if (pid > 0)
    { // parent writes to shared memory
        NonsenseWriter("in.txt");
        signal(SIGUSR2, HandleItemsRead);
        CopyFileToSHM("in.txt", str, pid);
    }

    if (shmdt(str) == -1)
    {
        perror("shmdt failed");
        return 1;
    }

    return 0;
}