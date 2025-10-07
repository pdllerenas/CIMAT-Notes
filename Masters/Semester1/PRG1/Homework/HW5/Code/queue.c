#include "queue.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

HuffmanNode *hn_create_node(unsigned char c, int freq) {
  HuffmanNode *hn = malloc(sizeof(HuffmanNode));
  if (!hn) {
    fprintf(stderr, "[huffman-node] Could not allocate memory\n");
    return NULL;
  }
  hn->c = c;
  hn->freq = freq;
  hn->left = hn->right = NULL;
  return hn;
}

PriorityQueue *pq_create_queue(int capacity) {
  PriorityQueue *pq = malloc(sizeof(PriorityQueue));
  if (!pq) {
    fprintf(stderr, "[priority-queue] Could not allocate memory\n");
    return NULL;
  }
  pq->capacity = capacity;
  pq->size = 0;
  pq->items = malloc(capacity * sizeof(HuffmanNode *));
  if (!pq->items) {
    fprintf(stderr, "[priority-queue] Could not allocate memory\n");
    free(pq);
    return NULL;
  }
  return pq;
}

void swap(HuffmanNode **a, HuffmanNode **b) {
  HuffmanNode *temp = *a;
  *a = *b;
  *b = temp;
}

void heapify_up(PriorityQueue *pq, int index) {
  // swap with parent if parent is bigger, to mantain min-heap property
  if (index && pq->items[(index - 1) / 2]->freq > pq->items[index]->freq) {
    swap(&pq->items[(index - 1) / 2], &pq->items[index]);
    // recursive call to compare with new parent node
    heapify_up(pq, (index - 1) / 2);
  }
}

void pq_enqueue(PriorityQueue *pq, HuffmanNode *node) {
  if (pq->size == pq->capacity) {
    fprintf(stderr, "[priority-queue] Priority queue is full\n");
    return;
  }

  // set items[size] to value, then update size
  pq->items[pq->size++] = node;

  // ensure heap property after insertion
  heapify_up(pq, pq->size - 1);
}

// used when a node is deleted
void heapify_down(PriorityQueue *pq, int index) {
  int smallest = index;
  int left = 2 * index + 1;
  int right = 2 * index + 2;

  // ensure left node (if exists) goes to top when smaller
  if (left < pq->size && pq->items[left]->freq < pq->items[smallest]->freq) {
    smallest = left;
  }

  // if right is smaller then both left and index, send it to the top
  if (right < pq->size && pq->items[right]->freq < pq->items[smallest]->freq) {
    smallest = right;
  }

  // only swap if children were smaller
  if (smallest != index) {
    swap(&pq->items[index], &pq->items[smallest]);
    // recursive call to compare swapped value with its new children
    heapify_down(pq, smallest);
  }
}

HuffmanNode *dequeue(PriorityQueue *pq) {
  if (pq->size == 0) {
    fprintf(stderr, "[priority-queue] Queue is empty\n");
    return NULL;
  }
  // grab smallest item (highest priority)
  HuffmanNode *item = pq->items[0];

  // set new root element to last item (this preserves complete tree property)
  pq->items[0] = pq->items[--pq->size];

  // ensure heap property by doing a top-down heapify
  heapify_down(pq, 0);
  return item;
}

HuffmanNode *peek(PriorityQueue *pq) {
  if (pq->size == 0) {
    fprintf(stderr, "[priority-queue] Queue is empty\n");
    return NULL;
  }
  return pq->items[0];
}

void pq_print_queue(PriorityQueue *pq) {
  for (int i = 0; i < pq->size; i++) {
    printf("%d, ", pq->items[i]->freq);
  }
}
