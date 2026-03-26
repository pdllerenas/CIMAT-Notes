#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

int main(void)
{
  struct sockaddr_in local, client;
  int s;
  int s1;
  int rc;
  int oldflags;
  char buf[256];

  local.sin_family = AF_INET;
  local.sin_port = htons(7500);
  local.sin_addr.s_addr = htonl(INADDR_ANY);

  s = socket(AF_INET, SOCK_STREAM, 0);
  if (s < 0)
  {
    perror("socket call failed");
    exit(1);
  }

  rc = bind(s, (struct sockaddr *)&local, sizeof(local));
  if (rc < 0)
  {
    perror("bind call failed");
    exit(1);
  }

  rc = listen(s, 5);
  if (rc)
  {
    perror("listen call failed");
    exit(1);
  }

  while (1)
  {
    socklen_t client_len = sizeof(client);
    s1 = accept(s, (struct sockaddr *)&client, &client_len);
    if (s1 < 0)
    {
      perror("accept call failed");
      exit(1);
    }
    printf("Client %s:%d connected.\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
    // nonblock stdin
    if ((oldflags = fcntl(0, F_GETFL, 0)) < 0)
    {
      exit(1);
    }
    if (fcntl(0, F_SETFL, oldflags | O_NONBLOCK) < 0)
    {
      exit(1);
    }

    // nonblock socket
    if ((oldflags = fcntl(s1, F_GETFL, 0)) < 0)
    {
      exit(1);
    }
    if (fcntl(s1, F_SETFL, oldflags | O_NONBLOCK) < 0)
    {
      exit(1);
    }

    while (1)
    {
      rc = recv(s1, buf, sizeof(buf) - 1, 0);
      if (rc > 0)
      {
        buf[rc] = '\0';
        printf("Client: %s", buf);
        if (strncasecmp(buf, "adios", 5) == 0)
          break;
      }
      else if (rc == 0)
      {
        printf("Client disconnected.\n");
        break;
      }
      else if (rc < 0 && errno != EAGAIN && errno != EWOULDBLOCK)
      {
        perror("recv failed");
        exit(1);
      }

      rc = read(0, buf, sizeof(buf) - 1);
      if (rc > 0)
      {
        buf[rc] = '\0';
        send(s1, buf, rc, 0);
        if (strncasecmp(buf, "adios", 5) == 0)
          break;
      }
      else if (rc < 0 && errno != EAGAIN && errno != EWOULDBLOCK)
      {
        perror("read failed");
        exit(1);
      }
      usleep(20000);
    }
  }
  exit(0);
}
