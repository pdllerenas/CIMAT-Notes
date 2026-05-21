#include <alsa/asoundlib.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define SAMPLE_RATE 44100
#define CHUNK_SIZE 512

#define TYPE_AUDIO 0
#define TYPE_REGISTER 1
#define TYPE_TEXT 2

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    printf("Usage: %s <Server_IP> <Port>\n", argv[0]);
    exit(1);
  }

  int s = socket(AF_INET, SOCK_DGRAM, 0);
  struct sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_port = htons(atoi(argv[2]));
  server.sin_addr.s_addr = inet_addr(argv[1]);
  connect(s, (struct sockaddr *)&server, sizeof(server));

  char name[32];
  printf("Enter your name: ");
  if (fgets(name, sizeof(name), stdin) != NULL)
  {
    name[strcspn(name, "\n")] = 0;
  }

  char reg_pkt[33];
  reg_pkt[0] = TYPE_REGISTER;
  strncpy(reg_pkt + 1, name, 31);
  send(s, reg_pkt, 1 + strlen(name) + 1, 0);

  // make socket non-blocking
  int flags = fcntl(s, F_GETFL, 0);
  fcntl(s, F_SETFL, flags | O_NONBLOCK);

  snd_pcm_t *capture_handle;
  snd_pcm_t *playback_handle;
  // if we are using wsl
  const char *device_name = (getenv("WSL_DISTRO_NAME") != NULL) ? "pulse" : "default";

  snd_pcm_open(&capture_handle, device_name, SND_PCM_STREAM_CAPTURE, 0);
  snd_pcm_set_params(capture_handle, SND_PCM_FORMAT_S16_LE, SND_PCM_ACCESS_RW_INTERLEAVED, 1, SAMPLE_RATE, 1, 50000);

  snd_pcm_open(&playback_handle, device_name, SND_PCM_STREAM_PLAYBACK, 0);
  snd_pcm_set_params(playback_handle, SND_PCM_FORMAT_S16_LE, SND_PCM_ACCESS_RW_INTERLEAVED, 1, SAMPLE_RATE, 1, 50000);

  printf("Registered...\n");

  short mic_buffer[CHUNK_SIZE];

  // buf for sending
  char out_buf[1 + (CHUNK_SIZE * sizeof(short))];
  out_buf[0] = TYPE_AUDIO;

  char in_buf[2048];

  while (1)
  {
    // read from mic
    int rc = snd_pcm_readi(capture_handle, mic_buffer, CHUNK_SIZE);
    if (rc == -EPIPE)
    {
      snd_pcm_prepare(capture_handle);
    }
    else if (rc > 0)
    {
      int audio_bytes = rc * sizeof(short);
      memcpy(out_buf + 1, mic_buffer, audio_bytes);
      send(s, out_buf, 1 + audio_bytes, 0);
    }

    rc = recv(s, in_buf, sizeof(in_buf), 0);
    if (rc > 0)
    {
      int type = in_buf[0];

      if (type == TYPE_AUDIO)
      {
        int audio_bytes = rc - 1;
        int frames_to_play = audio_bytes / sizeof(short);

        int play_rc = snd_pcm_writei(playback_handle, in_buf + 1, frames_to_play);
        if (play_rc == -EPIPE)
          snd_pcm_prepare(playback_handle);
      }
      else if (type == TYPE_TEXT)
      {
        printf("%s", in_buf + 1);
      }
    }
  }

  snd_pcm_close(capture_handle);
  snd_pcm_close(playback_handle);
  close(s);
  return 0;
}