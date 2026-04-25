#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

typedef enum
{
  EMPTY,
  UNREGISTERED,
  REGISTERED
} ClientState;
typedef struct
{
  ClientState state;
  char name[32];
} Client;


void broadcast_user_list(int max_sock, int s, fd_set *master_set, Client *clients)
{
  char announce[512] = "\n=== Active users ===\n";
  
  for (int k = 0; k <= max_sock; k++)
  {
    if (FD_ISSET(k, master_set) && k != s && clients[k].state == REGISTERED)
    {
      strcat(announce, clients[k].name);
      strcat(announce, "\n");
    }
  }
  strcat(announce, "====================\n");
  
  int len = strlen(announce);
  for (int j = 0; j <= max_sock; j++)
  {
    if (FD_ISSET(j, master_set) && j != s && clients[j].state == REGISTERED)
    {
      send(j, announce, len, 0);
    }
  }
}

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    printf("Usage: %s <user count>\n", argv[0]);
    exit(1);
  }
  int N = atoi(argv[1]);

  int s = socket(AF_INET, SOCK_STREAM, 0);
  if (s < 0)
  {
    perror("socket call failed");
    exit(1);
  }

  struct sockaddr_in local, client;

  local.sin_family = AF_INET;
  local.sin_port = htons(7500);
  local.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(s, (struct sockaddr *)&local, sizeof(local)))
  {
    perror("listen call failed");
    exit(1);
  }
  if (listen(s, N))
  {
    perror("listen call failed");
    exit(1);
  }

  fd_set master_set, read_set;
  FD_ZERO(&master_set);
  FD_ZERO(&read_set);

  FD_SET(s, &master_set);
  int max_sock = s;
  Client clients[FD_SETSIZE];
  memset(clients, 0, sizeof(clients));

  while (1)
  {
    read_set = master_set;

    if (select(max_sock + 1, &read_set, NULL, NULL, NULL) == -1)
    {
      perror("select error");
      exit(1);
    }
    for (int i = 0; i < max_sock + 1; i++)
    {
      if (FD_ISSET(i, &read_set))
      {
        if (i == s)
        {
          int new_client = accept(s, NULL, NULL);
          if (new_client > 0)
          {
            printf("New client connected (socket %d)\n", new_client);
            FD_SET(new_client, &master_set);
            if (new_client > max_sock)
            {
              max_sock = new_client;
            }
            clients[new_client].state = UNREGISTERED;
            memset(clients[new_client].name, 0, 32);
          }
        }
        else
        {
          char buf[256];
          memset(buf, 0, sizeof(buf));
          int nbytes = recv(i, buf, sizeof(buf) - 1, 0);

          if (nbytes <= 0)
          {
            if (nbytes == 0)
            {
              printf("Client on socket %d disconnected\n", i);
            }
            else
            {
              perror("recv call failed");
            }
            close(i);
            FD_CLR(i, &master_set);
            broadcast_user_list(max_sock, s, &master_set, clients);
          }
          else
          {
            buf[strcspn(buf, "\r\n")] = 0;
            if (clients[i].state == UNREGISTERED)
            {
              strncpy(clients[i].name, buf, 31);
              clients[i].state = REGISTERED;
              broadcast_user_list(max_sock, s, &master_set, clients);
            }
            else if (clients[i].state == REGISTERED)
            {
              char formatted_msg[512];
              int msg_len = snprintf(formatted_msg, sizeof(formatted_msg),
                                     "%s: %s\n", clients[i].name, buf);

              for (int j = 0; j <= max_sock; j++)
              {
                if (FD_ISSET(j, &master_set) && j != s && j != i &&
                    clients[j].state == REGISTERED)
                {
                  send(j, formatted_msg, msg_len, 0);
                }
              }
            }
          }
        }
      }
    }
  }
  exit(0);
}
