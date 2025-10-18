#include "hashtable.h"
#include "tree_node.h"
#include <stdio.h>

void show_menu() {
  printf("\n=== HASH TABLE MENU ===\n");
  printf("1. Insert value\n");
  printf("2. Delete value\n");
  printf("3. Print table\n");
  printf("4. Find maximum tree depth\n");
  printf("5. Exit\n");
  printf("Choose an option: ");
}

int main(int argc, char *argv[]) {
  int initial_capacity;
  printf("Enter initial hash table capacity: ");
  scanf("%d", &initial_capacity);

  HashTable *ht = hashtable_innit(initial_capacity);
  if (!ht) {
    fprintf(stderr, "Error: could not initialize hash table.\n");
    return 1;
  }

  int choice, value;
  int index;
  while (1) {
    show_menu();
    if (scanf("%d", &choice) != 1) {
      printf("Invalid input.\n");
      break;
    }

    switch (choice) {
    case 1: // Insert
      printf("Enter value to insert: ");
      scanf("%d", &value);
      hashtable_insert_node(&ht, value);
      printf("Inserted %d.\n", value);
      break;

    case 2: // Delete
      printf("Enter value to delete: ");
      scanf("%d", &value);
      hashtable_delete_node(ht, value);
      printf("Deleted %d (if it existed).\n", value);
      break;

    case 3: // Print
      printf("\nHash table contents:\n");
      hashtable_print(ht);
      break;

    case 4: // Find max
			printf("\nMAX TREE: ");
			TreeNode *max = find_max_depth_tree(ht, &index);
			printf("%d ", index);
      print_inorder(max);
			printf("\n");
      break;

    case 5: // Exit
      free_hashtable(ht);
      return 0;

    default:
      printf("Invalid option. Try again.\n");
    }
  }

  free_hashtable(ht);
  return 0;
}
