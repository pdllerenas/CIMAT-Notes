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
  return pq;
}

HuffmanNode *huffman_tree_parent_node(PriorityQueue *heap) {
  while (heap->size > 1) {
    HuffmanNode *h1 = dequeue(heap);
    HuffmanNode *h2 = dequeue(heap);
    HuffmanNode *hp = hn_create_node(' ', h1->freq + h2->freq);
    hp->left = h1;
    hp->right = h2;
    pq_enqueue(heap, hp);
  }
  return heap->items[0];
}

void print_huffman_tree(HuffmanNode *root, int depth) {
  if (!root)
    return;
  print_huffman_tree(root->right, depth + 1);
  for (int i = 0; i < depth; i++) {
    printf("    ");
  }
  if (root->right || root->left) {
    printf("+\n");
  } else {
    printf("%d\n", root->freq);
  }
  print_huffman_tree(root->left, depth + 1);
}
