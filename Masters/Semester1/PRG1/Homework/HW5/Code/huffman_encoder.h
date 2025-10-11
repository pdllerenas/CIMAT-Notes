#pragma once

#include "queue.h"
#include <stdint.h>

// we store the code a in a 64 bit type,
// but since not necessarily all bits are property of the code,
// we provide a length property to ensure ownership
typedef struct HuffmanCode {
  uint64_t code;
  uint8_t length;
} HuffmanCode;

HuffmanCode *innit_huffman_code();
int huffman_code_table(HuffmanNode *root, uint64_t code, uint8_t depth,
                       HuffmanCode **table);
HuffmanNode *huffman_tree_parent_node(PriorityQueue *heap);
PriorityQueue *enqueue_alphabet(int *frequencies, int size);
HuffmanNode *huffman_tree_parent_node(PriorityQueue *heap);
void print_huffman_tree(HuffmanNode *root, int depth);
void print_tree(HuffmanNode *node, int depth);
