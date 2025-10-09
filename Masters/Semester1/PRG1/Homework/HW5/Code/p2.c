#include "tree_node.h"
#include "hashtable.h"
#include <stdio.h>

int main(int argc, char *argv[]) { 
  TreeNode *test = tn_create_node(4);
  tn_insert_node(test, 8);
  tn_insert_node(test, 1);
  tn_insert_node(test, 3);
  tn_insert_node(test, 7);
  tn_insert_node(test, 2);
  print_inorder(test);

  HashTable *ht = hashtable_innit(5);

  hashtable_insert_node(ht, 8);
  hashtable_insert_node(ht, 3);
  hashtable_insert_node(ht, 4);
  hashtable_insert_node(ht, 6);
  hashtable_insert_node(ht, 1);
  hashtable_insert_node(ht, 2);
  hashtable_insert_node(ht, 9);
  hashtable_insert_node(ht, 12);

  hashtable_print(ht);

}
