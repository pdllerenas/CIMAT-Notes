#include "hashtable.h"
#include "tree_node.h"
#include <stdio.h>
#include <stdlib.h>

#define COLLISION_LIMIT 30

HashTable *hashtable_innit(int capacity) {
  HashTable *ht = malloc(sizeof(HashTable));
  ht->capacity = capacity;
  ht->hash = malloc(capacity * sizeof(TreeNode *));
  for (int i = 0; i < capacity; i++) {
    ht->hash[i] = malloc(sizeof(TreeNode));
  }
  return ht;
}

void hashtable_insert_node(HashTable *ht, int value) {
  tn_insert_node(ht->hash[value % ht->capacity], value);
}

void hashtable_print(HashTable *ht) {
  for (int i = 0; ht->capacity; i++) {
    print_inorder(ht->hash[i]);
    printf("\n");
  }
}

void hashtable_delete_node(HashTable *ht, int value) {}
