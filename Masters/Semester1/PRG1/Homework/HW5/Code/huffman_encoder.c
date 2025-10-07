#include "huffman_encoder.h"
#include "queue.h"
#include <stdio.h>

PriorityQueue *enqueue_alphabet(int *frequencies, int size) {
  // this priority queue will (initially) hold all image pixels, which is why we
  // set the capacity to the alphabet size
  PriorityQueue *pq = pq_create_queue(size);

  // loop over all alphabet
  for (int i = 0; i < size; i++) {
    if (frequencies[i] == 0) {
      continue;
    }
    HuffmanNode *hn = hn_create_node(i, frequencies[i]);
    if (!hn) {
      fprintf(stderr, "Error: Unable to enqueue alphabet.\n");
      return NULL;
    }
    pq_enqueue(pq, hn);
  }
  pq_print_queue(pq);
  return pq;
}

void generate_code_table(PriorityQueue *heap) {
  while (heap->size > 1) {
    dequeue(heap);

  }
}
