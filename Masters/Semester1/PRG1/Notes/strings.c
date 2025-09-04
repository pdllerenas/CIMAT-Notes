#include <stdio.h>
#include <string.h>

char* fn() {
  static char a[] = "Hello";
  return a;
}

int main() {
  char name[10];
  scanf("%s", name);
  int a = strcmp(name, name);
  puts(name);
}
