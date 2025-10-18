#include "huffman_encoder.h"
#include "pgm_io.h"
#include "queue.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

void printbits(uint64_t n, int length) {
  uint64_t i;
  i = 1UL << length;
  while (i > 0) {
    if (n & i)
      printf("1");
    else
      printf("0");
    i >>= 1;
  }
}

int huffman_compress(unsigned char **image, int rows, int cols,
                     const char *filename_out, HuffmanCode **table) {
  FILE *fp = fopen(filename_out, "wb");
  int bytes_written = 0;

  // code buffer
  uint64_t buffer = 0x0;

  // buffer bits used
  int written = 0;

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      unsigned char c = image[i][j];

      // assuming little endian
      // buffer adds the code for char c
      // we shift the code by 64-length-written,
      // thus, they are 'concatenated'. We first check if there is enough space
      if (64 - written >= table[c]->length) {
        buffer |= table[c]->code << (64 - written - table[c]->length);
        written += table[c]->length;
      } else {
        // get length - (64-written) msb of code,
        // that is, the msb of the code will be inserted into the buffer
        // and the rest will be stored for the next buffer iteration
        // missing = missing # of bits
        int missing = (table[c]->length - 64 + written);
        buffer |= (table[c]->code >> missing);
        fwrite(&buffer, sizeof(uint64_t), 1, fp);
        bytes_written += 8;
        // restart buffer with remaining bits from current code
        // this mask is 2^missing bits - 1, which is simply 1's at the position
        // of the remaining bits
        uint64_t remaining_mask = (1 << missing) - 1;
        buffer = (remaining_mask & table[c]->code << missing);
        written = missing;
      }
      // in case the last written bits complete 64 bits
      if (written == 64) {
        fwrite(&buffer, sizeof(uint64_t), 1, fp);
        buffer = 0x0;
        written = 0;
        bytes_written += 8;
      }
    }
  }
  // if bits did not align perfectly, simply write last buffer items
  if (buffer != 0) {
    fwrite(&buffer, sizeof(uint64_t), 1, fp);
    bytes_written += 8;
  }
  fclose(fp);
  return bytes_written;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Invalid argument count. Usage: %s <file-name> <file-out>\n", argv[0]);
    exit(1);
  }
  int *frequencies;
	char *file_out = argv[2];
  get_freq_table(argv[1], &frequencies);

  PriorityQueue *pq = enqueue_alphabet(frequencies, 256);
  HuffmanNode *huff_parent = huffman_tree_parent_node(pq);
  HuffmanCode **table = malloc(256 * sizeof(HuffmanCode *));
  for (int i = 0; i < 256; i++) {
    table[i] = innit_huffman_code();
  }

  int leafs = huffman_code_table(huff_parent, 0, 0, table);

  int *length_freq = calloc(32, sizeof(int));

  for (int i = 0; i < 256; i++) {
    length_freq[table[i]->length]++;
  }

  float expected_length = 0;
  for (int i = 0; i < 256; i++) {
    printf("%d --- ", i);
    printbits(table[i]->code, table[i]->length);
    printf("\n");
  }
  int sample_size = 0;
  for (int i = 0; i < 32; i++) {
    sample_size += length_freq[i];
    expected_length += (float)length_freq[i] * i;
  }
  expected_length /= (float)sample_size;

  int rows, cols;
  unsigned char **image = pgmRead(argv[1], &rows, &cols);
  int bytes_written =
      huffman_compress(image, rows, cols, file_out, table);
  // todo: decompress
  printf("original size: %lu bytes\n", rows * cols * sizeof(unsigned char));

  printf("expected length: %f\n", expected_length);
  printf("CR: %f\n",
         rows * cols * sizeof(unsigned char) / (float)bytes_written);
  printf("bpp: %f\n", (float)(8 * bytes_written) / (rows * cols));
}
