#pragma once
// TreeNode methods

typedef struct TreeNode {
  struct TreeNode *left, *right;
  int info;
} TreeNode;

TreeNode *tn_create_node(int val);
TreeNode *tn_insert_node(TreeNode *root, int val);
int tn_height(TreeNode *root);
void pprint_tree(TreeNode *root, int depth);
void print_inorder(TreeNode * root);
