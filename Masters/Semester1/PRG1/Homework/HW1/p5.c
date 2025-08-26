#include <stdio.h>
#define MAX_DIGITIS 10 // 2^31 is 10 digits

int main() {
  // inicializar en el char 0 (NULL)
  char input[MAX_DIGITIS] = {0}; 
  printf("Enter a number:\n");
  // guardar el string en el array input
  scanf("%s", input);

  // loop en direccion contraria
  for (int i = MAX_DIGITIS - 1; i >= 0; i--) {
    // imprimir solo si no es el caracter nulo
    if (input[i] != 0) {
      printf("%c", input[i]);
    }
  }
  printf("\n");
}
