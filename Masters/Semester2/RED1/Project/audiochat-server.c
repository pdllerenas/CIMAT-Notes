#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFER_SIZE 2048
#define MAX_CLIENTS 5

typedef struct
{
  int active;
  struct sockaddr_in addr;
} Client;

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    fprintf(stderr, "Usage: %s <port>\n", argv[0]);
    exit(0);
  }
  int s = socket(AF_INET, SOCK_DGRAM, 0);
  if (s < 0)
  {
    perror("socket call failed");
    exit(1);
  }

  struct sockaddr_in local;
  local.sin_family = AF_INET;
  local.sin_port = htons(argv[1]);
  local.sin_addr.s_addr = htonl(INADDR_ANY);

  int opt = 1;
  setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  if (bind(s, (struct sockaddr *)&local, sizeof(local)))
  {
    perror("bind call failed");
    exit(1);
  }

  Client clients[MAX_CLIENTS];
  memset(clients, 0, sizeof(clients));

  char buf[BUFFER_SIZE];
  struct sockaddr_in client_addr;
  socklen_t addr_len = sizeof(clients);

  printf("Listening on port %s\n", argv[1]);

  while (1)
  {
    int nbytes = recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr *)&client_addr, &addr_len);
    if (nbytes <= 0)
      continue;

    // check if client is new
    int client_idx = -1;
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
      if (clients[i].active &&
          clients[i].addr.sin_addr.s_addr == client_addr.sin_addr.s_addr &&
          clients[i].addr.sin_port == client_addr.sin_port)
      {
        client_idx = i;
        break;
      }
    }

    if (client_idx == -1)
    {
      for (int i = 0; i < MAX_CLIENTS; i++)
      {
        if (!clients[i].active)
        {
          clients[i].active = 1;
          clients[i].addr = client_addr;
          client_idx = i;
          printf("New client - %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
          break;
        }
      }
    }

    if (client_idx != -1)
    {
      for (int i = 0; i < MAX_CLIENTS; i++)
      {
        if (clients[i].active && i != client_idx)
        {
          sendto(s, buf, nbytes, 0, (struct sockaddr *)&clients[i].addr, sizeof(clients[i].addr));
        }
      }
    }
  }
  close(s);
  return 0;
}