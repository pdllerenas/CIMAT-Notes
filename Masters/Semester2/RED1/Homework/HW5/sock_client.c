#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

int main() {
  struct sockaddr_in server;
  int s;
  int rc;
  char buf[256];

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

  fgets(buf, sizeof(buf), stdin);

  rc = send(s, buf, sizeof(buf), 0);
  if (rc <= 0) {
    perror("send call failed");
    exit(1);
  }

  rc = recv(s, buf, sizeof(buf), 0);
  if (rc <= 0) {
    perror("recv call failed");
    exit(1);
  } else {
    printf("%s\n", buf);
  }
  exit(0);
}
