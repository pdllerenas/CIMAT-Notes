#include "tree_node.h"
typedef struct Stack {
  int top;
  unsigned int capacity;
  TreeNode **items;
} Stack;

Stack *create_stack(unsigned int capacity);
int isFull(Stack *s);
int isEmpty(Stack *s);
void push(Stack *s, TreeNode* val);
TreeNode* pop(Stack *s);
TreeNode* peek(Stack *s);
void free_stack(Stack *s);
void print_stack(Stack *s);
