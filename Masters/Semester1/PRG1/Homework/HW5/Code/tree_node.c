#include "tree_node.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

TreeNode *tn_create_node(int val) {
  TreeNode *t = malloc(sizeof(TreeNode));
  if (!t) {
    perror("Unable to allocate memory.\n");
    return NULL;
  }
  t->info = val;
  t->left = t->right = NULL;
  return t;
}

TreeNode *tn_insert_node(TreeNode *root, int val) {
  TreeNode *temp = root, *prev = NULL;
  if (root == NULL) {
    return tn_create_node(val);
  }
  while (temp != NULL) {
    prev = temp;
    if (val > temp->info) {
      temp = temp->right;
    } else {
      temp = temp->left;
    }
  }
  if (val > prev->info) {
    prev->right = tn_create_node(val);
  } else {
    prev->left = tn_create_node(val);
  }
  return root;
}

int tn_height(TreeNode *root) {
  if (!root) {
    return 0;
  }
  int l_depth = tn_height(root->left);
  int r_depth = tn_height(root->right);

  return (l_depth > r_depth ? l_depth : r_depth) + 1;
}

void pprint_tree(TreeNode *root, int depth) {
  if (!root)
    return;
  pprint_tree(root->right, depth + 1);
  for (int i = 0; i < depth; i++) {
    printf("    ");
  }
  printf("%d\n", root->info);
  pprint_tree(root->left, depth + 1);
}
