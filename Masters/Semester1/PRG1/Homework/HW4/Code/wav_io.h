#include <stdint.h>

// source: https://truelogic.org/wordpress/2015/09/04/parsing-a-wav-file-in-c/
// WAVE file header format
typedef struct __attribute__((__packed__)) {
  char riff[4];          // "RIFF"
  uint32_t overall_size; // file size - 8 bytes
  char wave[4];          // "WAVE"

  char fmt_chunk_marker[4]; // "fmt "
  uint32_t length_of_fmt;   // 16 for PCM
  uint16_t format_type;     // 1 = PCM (Linear quantization)
  uint16_t channels;        // 1 = mono, 2 = stereo
  uint32_t sample_rate;     // samples per second
  uint32_t byterate;        // SampleRate * NumChannels * BitsPerSample/8
  uint16_t block_align;     // NumChannels * BitsPerSample/8
  uint16_t bits_per_sample; // bits per sample

  char data_chunk_header[4]; // "data"
  uint32_t data_size;        // NumSamples * NumChannels * BitsPerSample/8
} HEADER;

HEADER *read_wav(const char *filename, char **wav_data);
int write_wav(const char *filename, int size, HEADER *header, char *wav);
