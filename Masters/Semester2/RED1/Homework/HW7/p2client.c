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
#define NUM_PACKETS 1000000

struct Payload
{
  uint32_t seq_number;
  char data[BUF_SIZE - 4];
};

void tcp(char *ip, char *port)
{
  struct Payload p;
  memset(p.data, 0, sizeof(p.data));

  struct sockaddr_in server;
  int s, rc;

  s = socket(AF_INET, SOCK_STREAM, 0);
  if (s < 0)
  {
    perror("socket call failed");
    exit(1);
  }

  int buffer_size = 5000 * 1024; 
  if (setsockopt(s, SOL_SOCKET, SO_SNDBUF, &buffer_size, sizeof(buffer_size)) < 0)
  {
    perror("setsockopt SO_SNDBUF failed");
  }

  server.sin_family = AF_INET;
  server.sin_port = htons(atoi(port));
  server.sin_addr.s_addr = inet_addr(ip);

  rc = connect(s, (struct sockaddr *)&server, sizeof(server));
  if (rc)
  {
    perror("connect call failed");
    exit(1);
  }

  for (int i = 1; i <= NUM_PACKETS; i++)
  {
    p.seq_number = htonl(i);
    send(s, &p, sizeof(p), 0);
  }
  close(s);
  printf("TCP Transmission Complete.\n");
}

void udp(char *ip, char *port)
{
  struct Payload p;
  memset(p.data, 0, sizeof(p.data));

  struct sockaddr_in server;
  int sockfd;

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
  {
    printf("Error: %s\n", strerror(errno));
    exit(0);
  }

  int buffer_size = 32 * 1024;
  if (setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &buffer_size, sizeof(buffer_size)) < 0)
  {
    perror("setsockopt SO_SNDBUF failed");
  }

  memset(&server, 0, sizeof(server));
  server.sin_family = AF_INET;

  if (inet_pton(AF_INET, ip, &server.sin_addr.s_addr) <= 0)
  {
    perror("inet_pton");
    exit(0);
  }
  server.sin_port = htons(atoi(port));

  for (int i = 1; i <= NUM_PACKETS; i++)
  {
    p.seq_number = htonl(i);
    sendto(sockfd, &p, sizeof(p), 0, (struct sockaddr *)&server, sizeof(server));
  }

  // termination packet
  sendto(sockfd, "", 0, 0, (struct sockaddr *)&server, sizeof(server));
  close(sockfd);
  printf("UDP Transmission Complete.\n");
}

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    printf("Usage: %s <ip> <port>\n", argv[0]);
    exit(1);
  }
  udp(argv[1], argv[2]);
  sleep(10);
  tcp(argv[1], argv[2]);
  return 0;
}