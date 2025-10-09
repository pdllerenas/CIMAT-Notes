#pragma once

#include "tree_node.h"

typedef struct HashTable {
  unsigned int capacity;
  TreeNode **hash;
} HashTable;

void hashtable_insert_node(HashTable *ht, int value);
HashTable *hashtable_innit(int capacity);
void hashtable_print(HashTable *ht);
