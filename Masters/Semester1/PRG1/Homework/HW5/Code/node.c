#include "node.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

static int count_ifs(int n) {
  if (n < 0)
    n = (n == INT_MIN) ? INT_MAX : -n;
  if (n < 10)
    return 1;
  if (n < 100)
    return 2;
  if (n < 1000)
    return 3;
  if (n < 10000)
    return 4;
  if (n < 100000)
    return 5;
  if (n < 1000000)
    return 6;
  if (n < 10000000)
    return 7;
  if (n < 100000000)
    return 8;
  if (n < 1000000000)
    return 9;
  /*      2147483647 is 2^31-1 - add more ifs as needed
  and adjust this final return as well. */
  return 10;
}

DoublyLinkedNode *dln_create_node(int val) {
  DoublyLinkedNode *pnode = malloc(sizeof(DoublyLinkedNode));
  pnode->info = val;
  pnode->prev = NULL;
  pnode->next = NULL;
  return pnode;
}

DoublyLinkedNode *dln_insert_before(DoublyLinkedNode *next, int data) {
  if (next == NULL) {
    fprintf(stderr, "Next pointer is NULL.\n");
    return NULL;
  }

  DoublyLinkedNode *np = dln_create_node(data);
  np->next = next;
  np->prev = next->prev;
  next->prev = np;
  return np;
}

DoublyLinkedNode *dln_insert_after(DoublyLinkedNode *prev, int data) {
  if (prev == NULL) {
    fprintf(stderr, "Previous pointer is NULL.\n");
    return NULL;
  }

  DoublyLinkedNode *np = dln_create_node(data);
  np->prev = prev;
  np->next = prev->next;
  prev->next = np;
  return np;
}

void dln_print_node(DoublyLinkedNode *np) {
  if (np->prev) {
    printf("%d", np->prev->info);
  } else {
    printf("NULL ");
  }
  printf("<--> ");
  printf("%d", np->info);
  printf(" <--> ");

  if (np->next) {
    printf("%d", np->next->info);
  } else {
    printf("NULL");
  }
  printf("\n");
}

void dln_print_node_list(DoublyLinkedNode *head) {
  DoublyLinkedNode *curr = head;

  // formatting for head pointer
  if (head->prev == NULL) {
    printf("         HEAD\n");
    printf("          |\n");
    printf("          v\n");
  } else {
    int prev_length = count_ifs(head->prev->info);

    printf("%*cHEAD\n", prev_length + 5, ' ');
    printf("%*c|\n", prev_length + 6, ' ');
    printf("%*cv\n", prev_length + 6, ' ');
  }

  // traverse linked list
  while (curr != NULL) {
    if (curr->prev == NULL) {
      printf("NULL");
    } else if (curr == head) {
      printf("%d", curr->prev->info);
    }
    // this indicates two-way pointer
    printf(" <--> ");
    printf("%d", curr->info);

    if (curr->next) {
      curr = curr->next;
    } else {
      printf(" <--> ");
      printf("NULL");
      break;
    }
  }
  printf("\n");
}

void dln_print_node_list_full(DoublyLinkedNode *head) {
  DoublyLinkedNode *curr = head;

  // if there are nodes on the left, traverse backwards until real head is found
  while (curr->prev != NULL) {
    curr = curr->prev;
  }

  dln_print_node_list(curr);
}


