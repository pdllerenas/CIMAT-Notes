#include "wav_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

HEADER *read_wav(const char *filename, char **wav_data) {
  HEADER *header = malloc(sizeof(HEADER));
  FILE *ptr;
  unsigned char buffer4[4];
  unsigned char buffer2[2];

  ptr = fopen(filename, "rb");

  int read = 0;

  // read header parts

  read = fread(header->riff, sizeof(header->riff), 1, ptr);

  read = fread(buffer4, sizeof(buffer4), 1, ptr);

  // convert little endian to big endian 4 byte int
  header->overall_size =
      buffer4[0] | (buffer4[1] << 8) | (buffer4[2] << 16) | (buffer4[3] << 24);

  read = fread(header->wave, sizeof(header->wave), 1, ptr);

  read =
      fread(header->fmt_chunk_marker, sizeof(header->fmt_chunk_marker), 1, ptr);

  read = fread(buffer4, sizeof(buffer4), 1, ptr);

  // convert little endian to big endian 4 byte integer
  header->length_of_fmt =
      buffer4[0] | (buffer4[1] << 8) | (buffer4[2] << 16) | (buffer4[3] << 24);

  read = fread(buffer2, sizeof(buffer2), 1, ptr);

  header->format_type = buffer2[0] | (buffer2[1] << 8);
  char format_name[10] = "";
  if (header->format_type == 1)
    strcpy(format_name, "PCM");
  else if (header->format_type == 6)
    strcpy(format_name, "A-law");
  else if (header->format_type == 7)
    strcpy(format_name, "Mu-law");

  read = fread(buffer2, sizeof(buffer2), 1, ptr);

  header->channels = buffer2[0] | (buffer2[1] << 8);

  read = fread(buffer4, sizeof(buffer4), 1, ptr);

  header->sample_rate =
      buffer4[0] | (buffer4[1] << 8) | (buffer4[2] << 16) | (buffer4[3] << 24);

  read = fread(buffer4, sizeof(buffer4), 1, ptr);

  header->byterate =
      buffer4[0] | (buffer4[1] << 8) | (buffer4[2] << 16) | (buffer4[3] << 24);

  read = fread(buffer2, sizeof(buffer2), 1, ptr);

  header->block_align = buffer2[0] | (buffer2[1] << 8);

  read = fread(buffer2, sizeof(buffer2), 1, ptr);

  header->bits_per_sample = buffer2[0] | (buffer2[1] << 8);

  read = fread(header->data_chunk_header, sizeof(header->data_chunk_header), 1,
               ptr);

  read = fread(buffer4, sizeof(buffer4), 1, ptr);

  header->data_size =
      buffer4[0] | (buffer4[1] << 8) | (buffer4[2] << 16) | (buffer4[3] << 24);

  *wav_data = malloc(header->data_size * sizeof(char));
  read = fread(*wav_data, header->data_size, 1, ptr);

  fclose(ptr);
  return header;
}

int write_wav(const char *filename, int size, HEADER *header, char *wav) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) return -1;

    // Update sizes
    header->data_size = size;
    header->overall_size = 36 + header->data_size;

    fwrite(header, sizeof(HEADER), 1, fp);
    fwrite(wav, 1, size, fp);

    fclose(fp);
    return 0;
}
