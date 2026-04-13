#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

#include "ChatPacket.h"
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

int main() {
  struct sockaddr_in server;
  int s;
  int rc;
  char buf[256];
  int oldflags;

  server.sin_family = AF_INET;
  server.sin_port = htons(7500);
  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  s = socket(AF_INET, SOCK_STREAM, 0);
  if (s < 0) {
    perror("socket call failed");
    exit(1);
  }
  rc = connect(s, (struct sockaddr *)&server, sizeof(server));

  if (rc) {
    perror("connect call failed");
    exit(1);
  }

  printf("Enter a name: \n");
  // non block stdin
  if ((oldflags = fcntl(0, F_GETFL, 0)) < 0) {
    exit(1);
  }
  if (fcntl(0, F_SETFL, O_NONBLOCK) < 0) {
    exit(1);
  }

  // non block socket
  if ((oldflags = fcntl(s, F_GETFL, 0)) < 0) {
    exit(1);
  }
  if (fcntl(s, F_SETFL, O_NONBLOCK) < 0) {
    exit(1);
  }

  while (1) {
    rc = read(0, buf, sizeof(buf) - 1);
    if (rc > 0) {
      buf[rc] = '\0';
      send(s, buf, rc, 0);
    } else if (rc == 0) {
      printf("Input closed...\n");
      break;
    } else if (rc < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
      perror("read failed");
      exit(1);
    }

    rc = recv(s, buf, sizeof(buf) - 1, 0);
    if (rc > 0) {
      buf[rc] = '\0';
      printf("%s", buf);
    } else if (rc == 0) {
      printf("Connection closed by server.\n");
      break;
    } else if (rc < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
      perror("recv call failed");
      exit(1);
    }
    usleep(20000);
  }
  exit(0);
}
