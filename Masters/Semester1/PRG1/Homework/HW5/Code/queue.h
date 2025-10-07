#pragma once
#include "tree_node.h"

typedef struct HuffmanNode{
  int freq;
  unsigned char c;
} HuffmanNode;

typedef struct {
  unsigned int capacity;
  unsigned int size;
  HuffmanNode **items;
} PriorityQueue;


HuffmanNode *hn_create_node(unsigned char c, int freq);
void heapify_up(PriorityQueue *pq, int index);
void pq_enqueue(PriorityQueue *pq, HuffmanNode* node);
void heapify_down(PriorityQueue *pq, int index);
PriorityQueue *pq_create_queue(int capacity);
HuffmanNode* dequeue(PriorityQueue *pq);
HuffmanNode* peek(PriorityQueue *pq);
void pq_print_queue(PriorityQueue *pq);
