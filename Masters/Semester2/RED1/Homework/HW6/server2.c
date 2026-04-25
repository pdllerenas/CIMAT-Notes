#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

typedef enum
{
  EMPTY,        // not a client (disconnected)
  UNREGISTERED, // user has not entered a name
  REGISTERED,   // user has entered a name
  PAIRED        // user has been paired to another client
} ClientState;

typedef struct Client
{
  ClientState state; // state of client, depending on actions done
  struct in_addr ip; // client ip
  int socket;        // socket assigned by server
  int paired_socket; // socket we will be connecting to for p2p connection
  int p2p_port;      // client's listening port for p2p connection
  char name[32];     // client name
} Client;

/*
  sends a list of all connected users
*/
void broadcast_user_list(int max_sock, int s, fd_set *master_set, Client *clients)
{
  char announce[2048];
  strcpy(announce, "\n=== Active users ===\n");

  for (int k = 0; k <= max_sock; k++)
  {
    if (FD_ISSET(k, master_set) && k != s)
    {
      if (clients[k].state == REGISTERED)
      {
        strncat(announce, clients[k].name, sizeof(announce) - strlen(announce) - 2);
        strncat(announce, "\n", sizeof(announce) - strlen(announce) - 1);
      }
      else if (clients[k].state == PAIRED)
      {
        strncat(announce, clients[k].name, sizeof(announce) - strlen(announce) - 2);
        strncat(announce, " (Busy)\n", sizeof(announce) - strlen(announce) - 1);
      }
    }
  }
  strncat(announce, "====================\n", sizeof(announce) - strlen(announce) - 1);

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

  int opt = 1;
  setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  struct sockaddr_in local, client;
  local.sin_family = AF_INET;
  local.sin_port = htons(7500);
  local.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(s, (struct sockaddr *)&local, sizeof(local)))
  {
    perror("bind call failed");
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

  printf("Server ready.\n");

  while (1)
  {
    read_set = master_set;

    if (select(max_sock + 1, &read_set, NULL, NULL, NULL) == -1)
    {
      perror("select error");
      exit(1);
    }

    for (int i = 0; i <= max_sock; i++)
    {
      if (FD_ISSET(i, &read_set))
      {
        if (i == s)
        {
          socklen_t addr_len = sizeof(client);
          int new_client = accept(s, (struct sockaddr *)&client, &addr_len);
          if (new_client > 0)
          {
            printf("New client connected (socket %d)\n", new_client);
            FD_SET(new_client, &master_set);
            if (new_client > max_sock)
            {
              max_sock = new_client;
            }
            clients[new_client].state = UNREGISTERED;
            clients[new_client].socket = new_client;
            clients[new_client].ip = client.sin_addr;
            clients[new_client].paired_socket = -1;
            clients[new_client].p2p_port = 0;
            memset(clients[new_client].name, 0, sizeof(clients[new_client].name));
          }
        }
        else
        {
          Client *sender = &clients[i];

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

            if (sender->state == PAIRED)
            {
              int partner = sender->paired_socket;
              if (partner != -1 && clients[partner].state == PAIRED)
              {
                clients[partner].state = REGISTERED;
                clients[partner].paired_socket = -1;
                char *msg = "\nYour peer disconnected. You are available again.\n";
                send(partner, msg, strlen(msg), 0);
                broadcast_user_list(max_sock, s, &master_set, clients);
              }
            }

            close(i);
            FD_CLR(i, &master_set);

            ClientState old_state = sender->state;
            memset(sender, 0, sizeof(Client));
            sender->state = EMPTY;

            // send list of active users when client disconnects and was registered
            if (old_state == REGISTERED)
            {
              broadcast_user_list(max_sock, s, &master_set, clients);
            }
          }
          else
          {
            buf[strcspn(buf, "\r\n")] = 0;

            if (sender->state == UNREGISTERED)
            {
              // parse name and port
              if (sscanf(buf, "%31s %d", sender->name, &sender->p2p_port) == 2)
              {
                sender->state = REGISTERED;
                printf("%s on P2P port %d\n", sender->name, sender->p2p_port);
                broadcast_user_list(max_sock, s, &master_set, clients);
              }
              else
              {
                char *err = "Error: Invalid registration.\n";
                send(i, err, strlen(err), 0);
              }
            }
            else if (sender->state == REGISTERED)
            {
              int user_found = 0;
              for (int j = 0; j <= max_sock; j++)
              {
                Client *receiver = &clients[j];

                if (FD_ISSET(j, &master_set) && j != s && j != i)
                {
                  if (strcmp(receiver->name, buf) == 0)
                  {
                    if (receiver->state == REGISTERED)
                    {
                      sender->state = PAIRED;
                      sender->paired_socket = receiver->socket;

                      receiver->state = PAIRED;
                      receiver->paired_socket = sender->socket;

                      // give sender the receiver's connection info
                      char peer_info[256];
                      int info_len = snprintf(peer_info, sizeof(peer_info), "PEER %s %d %s", inet_ntoa(receiver->ip), receiver->p2p_port, receiver->name);
                      send(sender->socket, peer_info, info_len, 0);

                      // notify receiver
                      char incoming_msg[256];
                      int inc_len = snprintf(incoming_msg, sizeof(incoming_msg), "\n%s is connecting to you...\n", sender->name);
                      send(receiver->socket, incoming_msg, inc_len, 0);

                      user_found = 1;
                      broadcast_user_list(max_sock, s, &master_set, clients);
                      break;
                    }
                    else if (receiver->state == PAIRED)
                    {
                      user_found = -1; // user is busy
                      break;
                    }
                  }
                }
              }

              if (user_found == 0)
              {
                char *err_msg = "Error: User not found.\n";
                send(i, err_msg, strlen(err_msg), 0);
              }
              else if (user_found == -1)
              {
                char *err_msg = "Error: User is already paired.\n";
                send(i, err_msg, strlen(err_msg), 0);
              }
            }
            else if (sender->state == PAIRED)
            {
              if (strncmp(buf, "RECONNECT", 9) == 0)
              {
                sender->state = REGISTERED;
                sender->paired_socket = 0;
                int partner = sender->paired_socket;

                clients[partner].paired_socket = -1;
                clients[partner].state = REGISTERED;
                sender->paired_socket = -1;

                broadcast_user_list(max_sock, s, &master_set, clients);
              }
            }
          }
        }
      }
    }
  }
  return 0;
}