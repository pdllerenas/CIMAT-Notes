#include "huffman_encoder.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

void print_bits(uint64_t n, int length) {
  uint64_t i;
  i = 1UL << (length - 1);
  while (i > 0) {
    if (n & i)
      printf("1");
    else
      printf("0");
    i >>= 1;
  }
}

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

HuffmanCode *innit_huffman_code() {
  HuffmanCode *hc = malloc(sizeof(HuffmanCode));
  hc->code = 0;
  hc->length = 0;
  return hc;
}

int huffman_code_table(HuffmanNode *root, uint64_t code, uint8_t depth,
                       HuffmanCode **table) {
  if (!root) {
    return 0;
  }

  int leaf_count = 0;
  // leaf node
  if (!root->left && !root->right) {
    // printf("%lu %d\n", code, depth);
    // print_bits(code, depth);
    // printf("\n");
    table[root->c]->code = code;
    table[root->c]->length = depth;
    leaf_count += 1;
  } else {
    // add 0 to left code
    leaf_count += huffman_code_table(root->left, code, depth + 1, table);

    // add 1 to right code
    leaf_count += huffman_code_table(root->right, code | (1ULL << depth),
                                     depth + 1, table);
  }
  return leaf_count;
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

void print_tree(HuffmanNode *node, int depth) {
  if (!node)
    return;
  for (int i = 0; i < depth; i++)
    putchar(' ');
  printf("%d\n", node->freq);
  print_tree(node->left, depth + 1);
  print_tree(node->right, depth + 1);
}
