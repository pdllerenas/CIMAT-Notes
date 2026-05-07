#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 1440
struct Payload
{
  uint32_t seq_number;
  char data[BUF_SIZE - 4];
};

void udp()
{
  struct Payload p;
  struct sockaddr_in local, client;
  int sockfd, n;
  socklen_t len;
  char buf[BUF_SIZE];

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
  {
    printf("ERROR: %s\n", strerror(errno));
    exit(1);
  }
  printf("Socket = %d\n", sockfd);

  memset(&local, 0, sizeof(local));
  local.sin_family = AF_INET;
  local.sin_addr.s_addr = htonl(INADDR_ANY);
  local.sin_port = htons(7500);

  if (bind(sockfd, (struct sockaddr *)&local, sizeof(local)) == -1)
  {
    printf("ERROR: %s\n", strerror(errno));
    exit(1);
  }

  memset(buf, 0, sizeof(buf));

  len = sizeof(client);

  if ((n = recvfrom(sockfd, &p, sizeof(p), 0, (struct sockaddr *)&client, &len)) < 0)
  {
    printf("ERROR: %s\n", strerror(errno));
    exit(1);
  }

  unsigned char expected_seq = 1;
  uint32_t received_seq = ntohl(p.seq_number);
  printf("received: %s\n", p.data);
  if (received_seq != expected_seq)
  {
    printf("Packet Loss Detected! Expected %u but got %u\n", expected_seq, received_seq);
    expected_seq = received_seq; // Resync
  }
  expected_seq++;
  close(sockfd);
}

int main()
{
  udp();
  return 0;
}