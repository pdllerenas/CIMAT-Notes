#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/types.h>

int main(void) {
  struct sockaddr_in local, client;
  int s;
  int s1;
  int rc;
  char buf[256];

  local.sin_family = AF_INET;
  local.sin_port = htons(7500);
  local.sin_addr.s_addr = htonl(INADDR_ANY);

  s = socket(AF_INET, SOCK_STREAM, 0);
  if (s < 0) {
    perror("socket call failed");
    exit(1);
  }

  rc = bind(s, (struct sockaddr *)&local, sizeof(local));
  if (rc < 0) {
    perror("bind call failed");
    exit(1);
  }

  rc = listen(s, 5);
  if (rc) {
    perror("listen call failed");
    exit(1);
  }

  s1 = accept(s, NULL, NULL);
  if (s1 < 0) {
    perror("accept call failed");
    exit(1);
  }

  int oldflags = fcntl(rc, F_GETFL, 0);
  if (oldflags < 0) {
    exit(1);
  }

  while (fgets(buf, sizeof(buf), stdin) != NULL) {
		if (strcmp(buf, "adios") == 0) {
			send(s1, buf, sizeof(buf), 0);
		}
    rc = recv(s1, buf, sizeof(buf), 0);
    if (rc <= 0) {
      perror("recv call failed");
      exit(1);
    }
    printf("%s\n", buf);
    rc = send(s1, buf, sizeof(buf), 0);
    if (rc <= 0) {
      perror("send fail called");
      exit(1);
    }
  }

  exit(0);

  // for (;;) {
  // 	printf("a");
  // }
}
