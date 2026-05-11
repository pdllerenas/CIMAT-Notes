#include <stdio.h>
#include <time.h>
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
#define NUM_BLOCKS 80

typedef struct
{
    size_t bytes;
    char data[SHM_SIZE];
} DataBlock;

typedef struct
{
    int offset;
    DataBlock blocks[NUM_BLOCKS];
} SharedData;

// do not store in cache, instead, read directly from memory
static volatile sig_atomic_t s = 0;

void handler(int signum)
{
    if (signum == SIGINT)
    {
        printf("Exiting program.\n");
        s = -1;
    }
    else if (signum == SIGUSR1)
    {
        printf("Data available in shared memory.\n");
        s = 1;
    }
    else if (signum == SIGUSR2)
    {
        printf("Data read by child.\n");
        s = 1;
    }
}

void CopyFileToSHM(const char *filename, SharedData *shm, pid_t pid)
{
    FILE *fptr = fopen(filename, "r");
    if (fptr == NULL)
    {
        perror("fopen failed");
        return;
    }

    shm->offset = -1;
    while (1)
    {
        // read data and copy to shm data, use offset
        size_t bytes_read = fread(shm->blocks[++shm->offset].data, 1, SHM_SIZE, fptr);
        shm->blocks[shm->offset].bytes = bytes_read;

        // notify child that data is ready
        kill(pid, SIGUSR1);

        // when EOF is reached, stop reading
        if (bytes_read == 0)
        {
            break;
        }

        // wait for child to finish reading
        s = 0;
        while (s == 0)
        {
            pause();
        }
    }

    // notify child that we are done processing file
    kill(pid, SIGINT);
    fclose(fptr);
}

void CopySHMToFile(const char *filename, SharedData *shm, pid_t pid)
{
    FILE *fptr = fopen(filename, "w");
    if (fptr == NULL)
    {
        perror("fopen failed");
        return;
    }

    while (1)
    {
        // await parent signal of done writing
        s = 0;
        while (s == 0)
        {
            pause();
        }

        // if parent signals no more data, exit function
        size_t bytes = shm->blocks[shm->offset].bytes;
        if (s == -1 || bytes == 0)
        {
            break;
        }

        // write newly read bytes
        fwrite(shm->blocks[shm->offset].data, 1, bytes, fptr);

        kill(pid, SIGUSR2);
    }

    fclose(fptr);
}

// write random text
void NonsenseWriter(const char *filename)
{
    srand(time(NULL));
    FILE *fptr = fopen(filename, "w");
    for (int i = 0; i < NUM_BLOCKS; i++)
    {
        for (int j = 0; j < SHM_SIZE; j++)
        {
            fputc('a' + (rand() % 26), fptr);
        }
        fputc('\n', fptr);
    }

    fclose(fptr);
}

int main(void)
{
    // get shared memory id, create if non existent
    int shmid = shmget(IPC_PRIVATE, sizeof(SharedData), 0666 | IPC_CREAT);
    if (shmid == -1)
    {
        perror("shmget failed");
        return 1;
    }

    // attach memory
    SharedData *shm = (SharedData *)shmat(shmid, NULL, 0);
    if (shm == (void *)-1)
    {
        perror("shmat failed");
        return 1;
    }

    struct sigaction sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);

    NonsenseWriter("in.txt");

    pid_t pid = fork();

    // child will read from shared memory
    if (pid == 0)
    {
        CopySHMToFile("out.txt", shm, getppid());
    }
    else if (pid > 0)
    {
        CopyFileToSHM("in.txt", shm, pid);
        wait(NULL); // wait for child
    }

    if (shmdt(shm) == -1)
    {
        perror("shmdt failed");
        return 1;
    }

    if (pid > 0)
    {
        shmctl(shmid, IPC_RMID, NULL);
        printf("Parent done.\n");
    }

    return 0;
}