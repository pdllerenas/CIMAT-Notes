#pragma once

#include "queue.h"

PriorityQueue *enqueue_alphabet(int *frequencies, int size);
HuffmanNode *huffman_tree_parent_node(PriorityQueue *heap);
void print_huffman_tree(HuffmanNode *root, int depth);
