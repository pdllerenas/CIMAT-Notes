#pragma once

#include "tree_node.h"

typedef struct HashTable {
  unsigned int capacity;
  TreeNode **hash;
} HashTable;

void hashtable_insert_node(HashTable **htp, int value);
HashTable *hashtable_innit(int capacity);
void hashtable_print(HashTable *ht);
void hashtable_delete_node(HashTable *ht, int value);
void free_hashtable(HashTable *ht);
TreeNode *find_max_depth_tree(HashTable *ht, int *index);
