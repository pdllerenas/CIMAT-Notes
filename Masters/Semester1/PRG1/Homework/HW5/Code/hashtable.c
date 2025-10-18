#include "hashtable.h"
#include "stack.h"
#include "tree_node.h"
#include <stdio.h>
#include <stdlib.h>

#define COLLISION_LIMIT 5

int is_prime(unsigned int n) {

  // Check if n is 1 or 0
  if (n <= 1)
    return 0;

  // Check if n is 2 or 3
  if (n == 2 || n == 3)
    return 1;

  // Check whether n is divisible by 2 or 3
  if (n % 2 == 0 || n % 3 == 0)
    return 0;

  // Check from 5 to square root of n
  // Iterate i by (i+6)
  for (int i = 5; i * i <= n; i = i + 6)
    if (n % i == 0 || n % (i + 2) == 0)
      return 0;

  return 1;
}

// finds the next prime after n
unsigned int next_prime(unsigned int n) {
  if (n < 2)
    return 2;
  n++;
  if (n <= 3)
    return n;
  n += (n % 2 == 0);
  while (1) {
    if (n % 3 != 0 && is_prime(n))
      return n;
    n += 2;
    if (n % 3 == 0)
      n += 2;
  }
}

HashTable *hashtable_innit(int capacity) {
  HashTable *ht = malloc(sizeof(HashTable));
  ht->capacity = capacity;
  ht->hash = malloc(capacity * sizeof(TreeNode *));
  return ht;
}

void free_hashtable(HashTable *ht) {
  for (int i = 0; i < ht->capacity; i++) {
    tn_free(ht->hash[i]);
  }
  free(ht);
}

void hashtable_insert_node(HashTable **htp, int value) {
  HashTable *ht = *htp;
  ht->hash[value % ht->capacity] =
      tn_insert_node(ht->hash[value % ht->capacity], value);
  int size = tn_size(ht->hash[value % ht->capacity]);

  if (size >= COLLISION_LIMIT) {
    // new table with twice the capacity (and extra, as we are trying to find a
    // prime number again)
    HashTable *new_ht = hashtable_innit(next_prime(2 * ht->capacity));
    for (int i = 0; i < ht->capacity; i++) {
      int h = tn_height(ht->hash[i]);
      Stack *s = create_stack(h);
      TreeNode *curr = ht->hash[i];
      while (curr || !isEmpty(s)) {
        while (curr) {
          push(s, curr);
          curr = curr->left;
        }
        curr = pop(s);
        // insert to new hash space
        new_ht->hash[curr->info % new_ht->capacity] = tn_insert_node(
            new_ht->hash[curr->info % new_ht->capacity], curr->info);
        curr = curr->right;
      }
      free_stack(s);
    }
    // free old hash table
    free_hashtable(*htp);
    *htp = new_ht;
  }
}

void hashtable_print(HashTable *ht) {
  for (int i = 0; i < ht->capacity; i++) {
		printf("%i: ", i);
    print_inorder(ht->hash[i]);
    printf("\n");
  }
}

void hashtable_delete_node(HashTable *ht, int value) {
  ht->hash[value % ht->capacity] =
      delete_node(ht->hash[value % ht->capacity], value);
}

/*
 *
 * Finds the deepest tree in the hash table.
 * ties are resolver with lowest index
 *
 */
TreeNode *find_max_depth_tree(HashTable *ht, int *index) {
	TreeNode *deepest_tree = ht->hash[0];
	int depth = tn_height(deepest_tree);
	*index = 0;
	for (int i = 1; i < ht->capacity; i++) {
		int n_depth = tn_height(ht->hash[i]);
		if (depth < n_depth) {
			deepest_tree = ht->hash[i];
			*index = i;
			depth = n_depth;
		}
	}
	return deepest_tree;
}
