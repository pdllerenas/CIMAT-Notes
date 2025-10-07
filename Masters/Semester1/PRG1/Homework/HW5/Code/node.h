#pragma once

typedef struct Node {
  struct Node *next;
  int info;
} Node;



// DoublyLinkedNode methods
typedef struct DoublyLinkedNode {
  struct DoublyLinkedNode *prev;
  int info;
  struct DoublyLinkedNode *next;
} DoublyLinkedNode;

DoublyLinkedNode *dln_create_node(int val);
DoublyLinkedNode *dln_insert_before(DoublyLinkedNode *next, int data);
DoublyLinkedNode *dln_insert_after(DoublyLinkedNode *prev, int data);
void dln_print_node(DoublyLinkedNode *np);

// DoublyLinkedNodeList methods
// typedef struct DoublyLinkedNodeList {
//   struct DoublyLinkedNode *head;
//   struct DoublyLinkedNode *tail;
// } DoublyLinkedNodeList;
//
void dln_print_node_list(DoublyLinkedNode *head);
void dln_print_node_list_full(DoublyLinkedNode *head);
