#include <alsa/asoundlib.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#define SAMPLE_RATE 44100
#define CHUNK_SIZE 512

int main(int argc, char *argv[])
{
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <ip> <port>\n", argv[0]);
  }
  int s = socket(AF_INET, SOCK_DGRAM, 0);
  if (s < 0)
  {
    perror("socket failed");
    exit(1);
  }

  struct sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_port = htons(argv[2]);
  server.sin_addr.s_addr = inet_addr(argv[1]);

  if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0)
  {
    perror("connect failed");
    exit(1);
  }

  // make socket non-blocking
  int flags = fcntl(s, F_GETFL, 0);
  fcntl(s, F_SETFL, flags | O_NONBLOCK);

  snd_pcm_t *capture_handle;
  snd_pcm_t *playback_handle;

  const char *device_name = "default";
  if (getenv("WSL_DISTRO_NAME") != NULL)
  {
    device_name = "pulse";
  }

  // Open capture (Microphone)
  snd_pcm_open(&capture_handle, device_name, SND_PCM_STREAM_CAPTURE, 0);
  snd_pcm_set_params(capture_handle, SND_PCM_FORMAT_S16_LE, SND_PCM_ACCESS_RW_INTERLEAVED, 1, SAMPLE_RATE, 1, 50000);

  // Open playback (Speakers)
  snd_pcm_open(&playback_handle, device_name, SND_PCM_STREAM_PLAYBACK, 0);
  snd_pcm_set_params(playback_handle, SND_PCM_FORMAT_S16_LE, SND_PCM_ACCESS_RW_INTERLEAVED, 1, SAMPLE_RATE, 1, 50000);

  printf("Speak\n");

  short buffer[CHUNK_SIZE];

  while (1)
  {
    int rc = snd_pcm_readi(capture_handle, buffer, CHUNK_SIZE);
    if (rc == -EPIPE)
    {
      snd_pcm_prepare(capture_handle);
    }
    else if (rc > 0)
    {
      int bytes_to_send = rc * sizeof(short);
      send(s, buffer, bytes_to_send, 0);
    }

    rc = recv(s, buffer, sizeof(buffer), 0);
    if (rc > 0)
    {
      int frames_to_play = rc / sizeof(short);
      int play_rc = snd_pcm_writei(playback_handle, buffer, frames_to_play);

      if (play_rc == -EPIPE)
      {
        snd_pcm_prepare(playback_handle);
      }
    }
  }

  snd_pcm_close(capture_handle);
  snd_pcm_close(playback_handle);
  close(s);
  return 0;
}