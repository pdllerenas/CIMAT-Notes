#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#define BUF_SIZE 1440
#define NUM_PACKETS 1000000

struct Payload
{
  uint32_t seq_number;
  char data[BUF_SIZE - 4];
};

int read_exactly(int sock, void *buf, size_t len)
{
  size_t total_read = 0;
  char *ptr = (char *)buf;
  while (total_read < len)
  {
    int n = recv(sock, ptr + total_read, len - total_read, 0);
    if (n <= 0)
      return n;
    total_read += n;
  }
  return total_read;
}

void tcp(char *port)
{
  struct Payload p;
  struct sockaddr_in local, client;
  int listen_sock, client_sock, n;
  socklen_t len;

  if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    printf("ERROR: %s\n", strerror(errno));
    exit(1);
  }

  int buffer_size = 5000 * 1024;
  if (setsockopt(listen_sock, SOL_SOCKET, SO_RCVBUF, &buffer_size, sizeof(buffer_size)) < 0)
  {
    perror("setsockopt SO_RCVBUF failed");
  }

  int opt = 1;
  setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  memset(&local, 0, sizeof(local));
  local.sin_family = AF_INET;
  local.sin_addr.s_addr = htonl(INADDR_ANY);
  local.sin_port = htons(atoi(port));

  if (bind(listen_sock, (struct sockaddr *)&local, sizeof(local)) == -1)
  {
    printf("ERROR Bind: %s\n", strerror(errno));
    exit(1);
  }

  if (listen(listen_sock, 5) == -1)
  {
    printf("ERROR Listen: %s\n", strerror(errno));
    exit(1);
  }
  len = sizeof(client);
  client_sock = accept(listen_sock, (struct sockaddr *)&client, &len);
  if (client_sock == -1)
  {
    printf("ERROR Accept: %s\n", strerror(errno));
    exit(1);
  }

  uint32_t expected_seq = 1;
  int timer_started = 0;
  struct timeval start_time, end_time;

  while ((n = read_exactly(client_sock, &p, sizeof(p))) > 0)
  {
    if (timer_started == 0)
    {
      gettimeofday(&start_time, NULL);
      timer_started = 1;
    }

    uint32_t received_seq = ntohl(p.seq_number);
    if (received_seq != expected_seq)
    {
      expected_seq = received_seq;
    }
    expected_seq++;
  }

  if (n == 0)
  {
    gettimeofday(&end_time, NULL);
    double diff = (end_time.tv_sec - start_time.tv_sec) +
                  (end_time.tv_usec - start_time.tv_usec) / 1000000.0;

    uint32_t packets_received = expected_seq - 1;
    double total_bytes_received = packets_received * 1440.0;
    double total_megabits = (total_bytes_received * 8.0) / 1000000.0;
    double mbps = total_megabits / diff;

    printf("--- TCP RESULTS ---\n");
    printf("Execution Time: %lf seconds\n", diff);
    printf("Throughput: %lf Megabits/sec\n", mbps);
  }

  close(client_sock);
  close(listen_sock);
}

void udp(char *port)
{
  struct Payload p;
  struct sockaddr_in local, client;
  int sockfd, n;
  socklen_t len;

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
  {
    printf("ERROR: %s\n", strerror(errno));
    exit(1);
  }

  int buffer_size = 32 * 1024;
  if (setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &buffer_size, sizeof(buffer_size)) < 0)
  {
    perror("setsockopt SO_RCVBUF failed");
  }

  memset(&local, 0, sizeof(local));
  local.sin_family = AF_INET;
  local.sin_addr.s_addr = htonl(INADDR_ANY);
  local.sin_port = htons(atoi(port));

  if (bind(sockfd, (struct sockaddr *)&local, sizeof(local)) == -1)
  {
    printf("ERROR: %s\n", strerror(errno));
    exit(1);
  }

  len = sizeof(client);
  uint32_t expected_seq = 1;

  int timer_started = 0;
  struct timeval start_time, end_time, tv;

  while (1)
  {
    n = recvfrom(sockfd, &p, sizeof(p), 0, (struct sockaddr *)&client, &len);

    if (n < 0)
    {
      // if timed out, this error triggers
      if (errno == EWOULDBLOCK || errno == EAGAIN)
      {
        // force termination if 0-packet took too long to arrive
        n = 0;
      }
      else
      {
        printf("ERROR: %s\n", strerror(errno));
        break;
      }
    }

    if (timer_started == 0 && n > 0)
    {
      gettimeofday(&start_time, NULL);
      timer_started = 1;

      // if wait more than 2 seconds for a packet, end the test.
      tv.tv_sec = 2;
      tv.tv_usec = 0;
      setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    }

    if (n == 0)
    {
      gettimeofday(&end_time, NULL);
      // subtract the 2-second timeout from our final time if we relied on it
      double diff = (end_time.tv_sec - start_time.tv_sec) +
                    (end_time.tv_usec - start_time.tv_usec) / 1000000.0;
      if (errno == EWOULDBLOCK || errno == EAGAIN)
      {
        diff -= 2.0;
      }

      uint32_t expected_total = NUM_PACKETS;
      uint32_t packets_received = expected_seq - 1;
      uint32_t lost_packets = expected_total - packets_received;
      double loss_percentage = ((double)lost_packets / expected_total) * 100.0;

      double total_bytes_received = packets_received * 1440.0;
      double total_megabits = (total_bytes_received * 8.0) / 1000000.0;
      double mbps = total_megabits / diff;

      printf("--- UDP RESULTS ---\n");
      printf("Execution Time: %lf seconds\n", diff);
      printf("Throughput: %lf Megabits/sec\n", mbps);
      printf("Lost Packets: %u (%.2f%%)\n", lost_packets, loss_percentage);
      break;
    }

    uint32_t received_seq = ntohl(p.seq_number);
    if (received_seq != expected_seq)
    {
      expected_seq = received_seq;
    }
    expected_seq++;
  }

  close(sockfd);
}

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    printf("Usage: %s <port>\n", argv[0]);
    exit(1);
  }

  udp(argv[1]);
  tcp(argv[1]);
  return 0;
}