#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

#include <sys/types.h>
#include <string.h>
#include <time.h>

int main()
{
  struct sockaddr_in server;
  int s;
  int rc;
  char buf[256];

  server.sin_family = AF_INET;
  server.sin_port = htons(7500);
  server.sin_addr.s_addr = inet_addr("127.0.0.1");
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
  struct tm *tm_info;
  char *endptr;

  rc = recv(s, buf, sizeof(buf), 0);
  if (rc <= 0)
  {
    perror("recv call failed");
    exit(1);
  }
  else
  {
    long seconds = strtol(buf, &endptr, 10);
    time_t my_time = time(NULL);

    time_t offset = seconds - my_time;
    time_t abs_offset = labs(offset);
    if (abs_offset > 60) {
      tm_info = localtime(&seconds);
      strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm_info);
      printf("Server time: %s\n", buf);

      tm_info = localtime(&my_time);
      strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm_info);
      printf("My time: %s\n", buf);

      printf("Offset: %lu seconds\n", abs_offset);

      my_time += offset;
      tm_info = localtime(&my_time);
      strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm_info);
      printf("New time: %s\n", buf);
    }
  }

  exit(0);
}
