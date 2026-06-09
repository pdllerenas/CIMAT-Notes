#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 2048

// Protocol packet types
#define TYPE_AUDIO 0
#define TYPE_REGISTER 1
#define TYPE_TEXT 2

typedef struct
{
  int active;
  struct sockaddr_in addr;
  char name[32];
} Client;

void broadcast_user_list(int s, Client *clients)
{
  char pkt[BUFFER_SIZE];
  // so client knows what is being sent
  pkt[0] = TYPE_TEXT;

  strcpy(pkt + 1, "\n=== Active Users ===\n");
  for (int i = 0; i < MAX_CLIENTS; i++)
  {
    if (clients[i].active)
    {
      strcat(pkt + 1, clients[i].name);
      strcat(pkt + 1, "\n");
    }
  }
  strcat(pkt + 1, "====================\n");

  int pkt_len = 1 + strlen(pkt + 1) + 1;

  for (int i = 0; i < MAX_CLIENTS; i++)
  {
    if (clients[i].active)
    {
      sendto(s, pkt, pkt_len, 0, (struct sockaddr *)&clients[i].addr, sizeof(clients[i].addr));
    }
  }
}

int main()
{
  int s = socket(AF_INET, SOCK_DGRAM, 0);
  if (s < 0)
  {
    perror("socket failed");
    exit(1);
  }

  struct sockaddr_in local;
  local.sin_family = AF_INET;
  local.sin_port = htons(7500);
  local.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(s, (struct sockaddr *)&local, sizeof(local)) < 0)
  {
    perror("bind failed");
    exit(1);
  }

  Client clients[MAX_CLIENTS];
  memset(clients, 0, sizeof(clients));

  printf("Listening on port 7500...\n");
  char buf[BUFFER_SIZE];
  struct sockaddr_in client_addr;
  socklen_t addr_len = sizeof(client_addr);

  while (1)
  {
    int nbytes = recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr *)&client_addr, &addr_len);
    if (nbytes <= 0)
      continue;

    // determine type of packet
    int type = buf[0];

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

    if (type == TYPE_REGISTER)
    {
      // new client
      if (client_idx == -1)
      {
        for (int i = 0; i < MAX_CLIENTS; i++)
        {
          if (!clients[i].active)
          {
            client_idx = i;
            clients[i].active = 1;
            clients[i].addr = client_addr;
            break;
          }
        }
      }
      if (client_idx != -1)
      {
        strncpy(clients[client_idx].name, buf + 1, 31);
        printf("Registered user: %s\n", clients[client_idx].name);
        broadcast_user_list(s, clients);
      }
    }
    else if (type == TYPE_AUDIO)
    {
      if (client_idx != -1)
      {
        sleep(1);
        for (int i = 0; i < MAX_CLIENTS; i++)
        {
          if (clients[i].active && i != client_idx)
          {
            sendto(s, buf, nbytes, 0, (struct sockaddr *)&clients[i].addr, sizeof(clients[i].addr));
          }
        }
      }
    }
  }
  return 0;
}