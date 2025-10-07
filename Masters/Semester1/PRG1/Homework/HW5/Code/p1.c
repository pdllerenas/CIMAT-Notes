#include "huffman_encoder.h"
#include "pgm_io.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Invalid argument count. Usage: %s <file-name>\n", argv[0]);
    exit(1);
  }
  int *frequencies;
  get_freq_table(argv[1], &frequencies);

  PriorityQueue *pq = enqueue_alphabet(frequencies, 256);
  HuffmanNode *huff_parent = huffman_tree_parent_node(pq);
  print_huffman_tree(huff_parent, 0);

}
