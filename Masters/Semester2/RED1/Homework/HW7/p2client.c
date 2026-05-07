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

#define BUF_SIZE 1440

struct Payload
{
  uint32_t seq_number;
  char data[BUF_SIZE - 4];
};

void tcp()
{

  struct sockaddr_in server;
  int s;
  int rc;
  char buf[BUF_SIZE];
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
  memset(buf, 52, BUF_SIZE - 1);
  buf[BUF_SIZE - 1] = '\0';
  rc = send(s, buf, sizeof(buf), 0);
  rc = recv(s, buf, sizeof(buf), 0);
}

void udp()
{
  struct Payload p;
  memset(p.data, 52, sizeof(p.data));

  struct sockaddr_in server;
  int sockfd, n;

  char server_addr[INET_ADDRSTRLEN] = {"127.0.0.1"};
  char buf[BUF_SIZE], buf1[BUF_SIZE];

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
  {
    printf("Error: %s\n", strerror(errno));
    exit(0);
  }
  printf("Socket = %d\n", sockfd);

  memset(&server, 0, sizeof(server));
  server.sin_family = AF_INET;

  if (inet_pton(AF_INET, server_addr, &server.sin_addr.s_addr) <= 0)
  {
    perror("inet_pton");
    exit(0);
  }
  server.sin_port = htons(7500);

  for (int i = 1; i <= 100; i++)
  {
    p.seq_number = htonl(i);
    printf("p = %u\n", p.seq_number);
    sendto(sockfd, &p, sizeof(p), 0, (struct sockaddr *)&server, sizeof(server));
  }

  close(sockfd);
}

int main()
{
  udp();
  return 0;
}