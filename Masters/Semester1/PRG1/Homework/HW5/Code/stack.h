typedef struct Stack {
  int top;
  unsigned int capacity;
  int *items;
} Stack;

Stack *create_stack(unsigned int capacity);
int isFull(Stack *s);
int isEmpty(Stack *s);
void push(Stack *s, int val);
int pop(Stack *s);
int peek(Stack *s);
void free_stack(Stack *s);
void print_stack(Stack *s);
