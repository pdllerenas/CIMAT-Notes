#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

int main(void)
{
  struct sockaddr_in local, client;
  int s;
  int s1;
  int rc;
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

  s1 = accept(s, NULL, NULL);
  if (s1 < 0)
  {
    perror("accept call failed");
    exit(1);
  }

  time_t seconds = time(NULL) + rand();

  snprintf(buf, sizeof(buf), "%ld", (long)seconds);

  rc = send(s1, buf, sizeof(buf), 0);
  if (rc <= 0)
  {
    perror("send fail called");
    exit(1);
  }
  exit(0);

  // for (;;) {
  // 	printf("a");
  // }
}
