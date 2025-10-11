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
    if (table[i]->length) {
      printf("%d --- ", i);
      printbits(table[i]->code, table[i]->length);
      printf("\n");
    }
  }

  int rows, cols;
  unsigned char **image = pgmRead(argv[1], &rows, &cols);

  FILE *fp = fopen("lena_huff.bin", "wb");

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      unsigned char c = image[i][j];
			fwrite(fp);
    }
  }
}
