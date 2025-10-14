#include "tree_node.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

int max(int a, int b) { return (a > b) ? a : b; }

DepthTreeNode *dtn_create_node(int val) {
  DepthTreeNode *dtn = malloc(sizeof(DepthTreeNode));
  dtn->info = val;
  dtn->left = dtn->right = NULL;
  dtn->depth = 1;
  return dtn;
}

int depth(DepthTreeNode *n) { return n ? n->depth : 0; }

DepthTreeNode *right_rotate(DepthTreeNode *y) {
  DepthTreeNode *x = y->left;
  DepthTreeNode *T2 = x->right;

  x->right = y;
  y->left = T2;

  y->depth = 1 + max(depth(y->left), depth(y->right));
  x->depth = 1 + max(depth(x->left), depth(x->right));
  return x;
}

DepthTreeNode *left_rotate(DepthTreeNode *x) {
  DepthTreeNode *y = x->right;
  DepthTreeNode *T2 = y->left;

  y->left = x;
  x->right = T2;

  x->depth = 1 + max(depth(x->left), depth(x->right));
  y->depth = 1 + max(depth(y->left), depth(y->right));
  return y;
}

int get_balance(DepthTreeNode *n) {
  if (!n)
    return 0;
  return depth(n->left) - depth(n->right);
}

DepthTreeNode *dtn_insert(DepthTreeNode *node, int val) {
  if (!node)
    return dtn_create_node(val);

  if (val < node->info) {
    node->left = dtn_insert(node->left, val);
  } else if (val > node->info) {
    node->right = dtn_insert(node->right, val);
  } else // Equal keys are not allowed in BST
    return node;

  node->depth = 1 + max(depth(node->left), depth(node->right));

  int balance = get_balance(node);

  // if not balanced (|balance| > 1)
  // we have 4 cases

  // left depth is bigger and left node is bigger
  if (balance > 1 && val < node->left->info) {
    return right_rotate(node);
  }

  // right depth is bigger and right node is smaller
  if (balance < -1 && val > node->right->info) {
    return left_rotate(node);
  }

  // left depth is bigger and left node is smaller
  if (balance > 1 && val > node->left->info) {
    node->left = left_rotate(node->left);
    return right_rotate(node);
  }

  // right depth is bigger and right node is smaller
  if (balance < -1 && val < node->right->info) {
    node->right = right_rotate(node->right);
    return left_rotate(node);
  }

  return node;
}

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

void store_inorder(TreeNode *root, int **nodes, int size) {
  if (!root) {
    return;
  }

  store_inorder(root->left, nodes, size + 1);
  *nodes[size] = root->info;
  store_inorder(root->right, nodes, size + 1);
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

void print_dtn_inorder(DepthTreeNode *root) {
  if (!root) {
    return;
  }
  print_dtn_inorder(root->left);
  printf("%d ", root->info);
  print_dtn_inorder(root->right);
}

void print_inorder(TreeNode *root) {
  if (!root) {
    return;
  }
  print_inorder(root->left);
  printf("%d ", root->info);
  print_inorder(root->right);
}

void tn_free(TreeNode *root) {
  if (!root) {
    return;
  }
  tn_free(root->left);
  tn_free(root->right);
  free(root);
}

void dtn_free(DepthTreeNode *root) {
  if (!root) {
    return;
  }
  dtn_free(root->left);
  dtn_free(root->right);
  free(root);
}
