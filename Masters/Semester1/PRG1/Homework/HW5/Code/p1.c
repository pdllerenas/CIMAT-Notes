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

unsigned char **huffman_decompress(const char *filename, HuffmanCode **table) {
	// TODO
	FILE *fp = fopen(filename, "rb");
	return NULL;
}

void huffman_compress(unsigned char **image, int rows, int cols, const char *filename_out, HuffmanCode **table) {
  FILE *fp = fopen(filename_out, "wb");
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
      }
    }
  }
	// if bits did not align perfectly, simply write last buffer items
	if (buffer != 0) {
		fwrite(&buffer, sizeof(uint64_t), 1, fp);
	}
	fclose(fp);

}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Invalid argument count. Usage: %s <file-name>\n", argv[0]);
    exit(1);
  }
  int *frequencies;
  get_freq_table(argv[1], &frequencies);

  PriorityQueue *pq = enqueue_alphabet(frequencies, 256);
  HuffmanNode *huff_parent = huffman_tree_parent_node(pq);
  HuffmanCode **table = malloc(256 * sizeof(HuffmanCode *));
  for (int i = 0; i < 256; i++) {
    table[i] = innit_huffman_code();
  }

  int leafs = huffman_code_table(huff_parent, 0, 0, table);

	for (int i = 0; i < 256; i++) {
		printf("%d --- ", i);
		printbits(table[i]->code, table[i]->length);
		printf("\n");
	}

  int rows, cols;
  unsigned char **image = pgmRead(argv[1], &rows, &cols);
	huffman_compress(image, rows, cols, "lena_huff.bin", table);
	unsigned char **decompressed = huffman_decompress("lena_huff.bin", table);

}
