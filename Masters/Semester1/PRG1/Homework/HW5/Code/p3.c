#include "stack.h"
#include "tree_node.h"
#include <stdio.h>
#include <stdlib.h>

TreeNode *random_tree() {
  int node_count = 1 + rand() % 10;

  int val = 1 + rand() % 1000;
  TreeNode *n = tn_create_node(val);

  for (int i = 0; i < node_count; i++) {
    val = 1 + rand() % 1000;
    tn_insert_node(n, val);
  }
  return n;
}

DepthTreeNode *merge_balance(TreeNode **tn, int n) {
  DepthTreeNode *dtn = NULL;

  for (int i = 0; i < n; i++) {
    int h = tn_height(tn[i]);
    Stack *s = create_stack(h);
    TreeNode *curr = tn[i];
    while (curr || !isEmpty(s)) {
      while (curr) {
        push(s, curr);
        curr = curr->left;
      }
      curr = pop(s);
      dtn = dtn_insert(dtn, curr->info);
      curr = curr->right;
    }
    free_stack(s);
  }
  return dtn;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Invalid argument count. Usage: %s <n-trees> <seed>\n",
            argv[0]);
    return 1;
  }
  int n = atoi(argv[1]);
  srand(atoi(argv[2]));

  TreeNode **tn = malloc(n * sizeof(TreeNode *));

  for (int i = 0; i < n; i++) {
    tn[i] = random_tree();
		printf("Tree %d\n", i);
    print_inorder(tn[i]);
    printf("\n");
  }

  DepthTreeNode *dtn = merge_balance(tn, n);
	printf("\nBalanced Tree:\n");
  print_dtn_inorder(dtn);
	printf("\n");

	// if |balance| > 1, tree is unbalanced
  int balance = get_balance(dtn);
  printf("balance: %d\n", balance);

  for (int i = 0; i < n; i++) {
    tn_free(tn[i]);
  }
	free(tn);
  dtn_free(dtn);
}
