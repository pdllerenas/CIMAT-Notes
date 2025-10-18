#pragma once
// TreeNode methods

typedef struct TreeNode {
  struct TreeNode *left, *right;
  int info;
} TreeNode;

typedef struct DepthTreeNode {
  struct DepthTreeNode *left, *right;
  int info;
  int depth;
} DepthTreeNode;

DepthTreeNode *dtn_create_node(int val);
DepthTreeNode *dtn_insert(DepthTreeNode *node, int val);
TreeNode *tn_create_node(int val);
TreeNode *tn_insert_node(TreeNode *root, int val);
int tn_height(TreeNode *root);
void pprint_tree(TreeNode *root, int depth);
void print_inorder(TreeNode *root);
void print_dtn_inorder(DepthTreeNode *root);
void tn_free(TreeNode *root);
void dtn_free(DepthTreeNode *root);
int depth(DepthTreeNode *n);
int get_balance(DepthTreeNode *n);
int tn_size(TreeNode *root);
TreeNode *delete_node(TreeNode *root, int val);
