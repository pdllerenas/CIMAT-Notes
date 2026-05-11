#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

int main()
{
  struct sockaddr_in server;
  int s;
  int rc;
  char buf[512];
  int oldflags;

  int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in listen_addr;
  listen_addr.sin_family = AF_INET;
  listen_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  listen_addr.sin_port = 7501;

  if (bind(listen_sock, (struct sockaddr *)&listen_addr, sizeof(listen_addr)) < 0)
  {
    perror("Bind failed");
    exit(1);
  }
  listen(listen_sock, 1);

  socklen_t len = sizeof(listen_addr);
  getsockname(listen_sock, (struct sockaddr *)&listen_addr, &len);
  int my_p2p_port = ntohs(listen_addr.sin_port);
  printf("Listening for P2P connections on port %d\n", my_p2p_port);

  fcntl(listen_sock, F_SETFL, O_NONBLOCK);

  server.sin_family = AF_INET;
  server.sin_port = htons(7500);
  server.sin_addr.s_addr = inet_addr("148.207.185.20");

  s = socket(AF_INET, SOCK_STREAM, 0);
  if (s < 0)
  {
    perror("socket call failed");
    exit(1);
  }

  rc = connect(s, (struct sockaddr *)&server, sizeof(server));
  if (rc)
  {
    perror("connect call failed");
    exit(1);
  }

  char my_name[32];
  memset(my_name, 0, sizeof(my_name));
  printf("Enter your name: \n");
  rc = read(0, my_name, sizeof(my_name) - 1);
  if (rc > 0)
  {
    my_name[rc] = '\0';
    my_name[strcspn(my_name, "\r\n")] = 0;

    char registration_msg[256];
    snprintf(registration_msg, sizeof(registration_msg), "%s %d", my_name, my_p2p_port);
    send(s, registration_msg, strlen(registration_msg), 0);
    // printf("Server: %s", buf);
  }

  // Set stdin to non-blocking
  if ((oldflags = fcntl(0, F_GETFL, 0)) < 0)
    exit(1);
  if (fcntl(0, F_SETFL, O_NONBLOCK) < 0)
    exit(1);

  // Set server socket to non-blocking
  if ((oldflags = fcntl(s, F_GETFL, 0)) < 0)
    exit(1);
  if (fcntl(s, F_SETFL, O_NONBLOCK) < 0)
    exit(1);

  // friend to pair with
  printf("Enter friend's name to pair with: \n");

  int p2p_sock = -1;
  char peer_name[32];
  memset(peer_name, 0, sizeof(peer_name));

  while (1)
  {
    if (p2p_sock == -1)
    {
      struct sockaddr_in peer_addr;
      socklen_t peer_len = sizeof(peer_addr);
      int new_conn = accept(listen_sock, (struct sockaddr *)&peer_addr, &peer_len);
      if (new_conn > 0)
      {
        p2p_sock = new_conn;
        recv(p2p_sock, peer_name, sizeof(peer_name), 0);
        printf("\n====You are now chatting with %s====\n", peer_name);

        fcntl(p2p_sock, F_SETFL, O_NONBLOCK);
      }
    }

    rc = read(0, buf, sizeof(buf) - 1);
    if (rc > 0)
    {
      buf[rc] = '\0';
      if (p2p_sock != -1)
      {
        if (strncmp(buf, "EXIT", 4) == 0)
        {
          printf("\nYou closed the connection.\n");
          send(p2p_sock, buf, rc, 0);
          close(p2p_sock);
          p2p_sock = -1;
          send(s, "RECONNECT", 9, 0);
          continue;
        }
        send(p2p_sock, buf, rc, 0);
      }
      else
      {
        send(s, buf, rc, 0);
      }
    }
    else if (rc == 0)
    {
      printf("Input closed.\n");
      break;
    }

    if (p2p_sock != -1)
    {
      rc = recv(p2p_sock, buf, sizeof(buf) - 1, 0);
      if (rc > 0)
      {
        buf[rc] = '\0';
        if (strncmp(buf, "EXIT", 4) == 0)
        {
          printf("\nPeer closed the connection.\n");
          close(p2p_sock);
          p2p_sock = -1;
        }
        else
        {
          printf("%s: %s", peer_name, buf);
        }
      }
      else if (rc == 0)
      {
        printf("\nPeer disconnected.\n");
        close(p2p_sock);
        p2p_sock = -1;
      }
    }
    else
    {
      rc = recv(s, buf, sizeof(buf) - 1, 0);
      if (rc > 0)
      {
        buf[rc] = '\0';

        if (strncmp(buf, "PEER ", 5) == 0)
        {
          char peer_ip[32];
          int peer_port;

          if (sscanf(buf, "PEER %31s %d %s", peer_ip, &peer_port, peer_name) == 3)
          {
            int new_sock = socket(AF_INET, SOCK_STREAM, 0);
            struct sockaddr_in peer_addr;
            peer_addr.sin_family = AF_INET;
            peer_addr.sin_port = htons(peer_port);
            peer_addr.sin_addr.s_addr = inet_addr(peer_ip);

            printf("\nConnecting to %s:%d...\n", peer_ip, peer_port);

            if (connect(new_sock, (struct sockaddr *)&peer_addr, sizeof(peer_addr)) == 0)
            {
              printf("\n====You are now chatting with %s====\n", peer_name);
              p2p_sock = new_sock;
              fcntl(p2p_sock, F_SETFL, O_NONBLOCK);
              // send my name to (peer) server
              send(p2p_sock, my_name, strlen(my_name), 0);
            }
            else
            {
              perror("P2P connect failed");
              close(new_sock);
            }
          }
        }
        else
        {
          printf("Server: %s", buf);
        }
      }
    }

    usleep(20000);
  }

  if (p2p_sock != -1)
    close(p2p_sock);
  close(s);
  close(listen_sock);
  exit(0);
}