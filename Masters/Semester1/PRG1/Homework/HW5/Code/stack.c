#include "stack.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

Stack *create_stack(unsigned int capacity) {
  Stack *s = malloc(sizeof(Stack));
  if (!s) {
    perror("[stack]: Unable to allocate memory.\n");
    return NULL;
  }
  s->capacity = capacity;
  s->top = -1;
  s->items = malloc(s->capacity * sizeof(int));
  if (!s->items) {
    perror("[stack]: Unable to allocate memory.\n");
    return NULL;
  }
  return s;
}

int isFull(Stack *s) { return s->top == s->capacity - 1; }

int isEmpty(Stack *s) { return s->top == -1; }

void push(Stack *s, int val) {
  if (isFull(s)) {
    fprintf(stderr, "[stack] Stack overflow\n");
    return;
  }
  // increment s->top first, then store value at (new) top
  s->items[++s->top] = val;
}

int pop(Stack *s) {
  if (isEmpty(s)) {
    fprintf(stderr, "[stack] Stack underflow\n");
    return INT_MIN;
  }
  // return top item, then decrement s->top
  return s->items[s->top--];
}

int peek(Stack *s) {
  if (isEmpty(s)) {
    fprintf(stderr, "[stack] Stack is empty\n");
    return INT_MIN;
  }
  // return top item, without decrementing top
  return s->items[s->top];
}

void free_stack(Stack *s) {
  if (s) {
    free(s->items);
    free(s);
  }
}

void print_stack(Stack *s) {
  for (int i = s->top; i >= 0; i--) {
    printf("%d\n", s->items[i]);
  }
}
