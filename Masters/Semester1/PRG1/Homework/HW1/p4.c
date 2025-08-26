#include <stdio.h>
#include <stdlib.h>

int main() {
  // ciclo infinito para hacer muchas pruebas sin tener que correr
  // el programa de nuevo
  while (1) {
    printf("type an operation between two fractions.\nformat: 'a/b(*,/,+,-)c/d'.\n\n");
    int a, b, c, d; // enteros que formaran parte de las fracciones a operar
    char op; // operador a utilizar
    int numerator, denominator; // variables con el unico fin de representar la fraccion en forma a/b
    // solamente entraremos al parseo de numeros si se han
    // introducido los 5 objetos en el formato correcto
    if (scanf("%d/%d%c%d/%d", &a, &b, &op, &c, &d) == 5) {
      // switch para determinar el operador insertado y realizar operacion
      // de numerador y denominador por separado
      switch (op) {
      case 42: // char 42 == '*'
        numerator = a * c;
        denominator = b * d;
        break;
      case 43: // char 43 == '+'
        numerator = a * d + b * c;
        denominator = b * d;
        break;
      case 45: // char 45 == '-'
        numerator = a * d - b * c;
        denominator = b * d;
        break;
      case 47: // char 47 == '/'
        numerator = a * d;
        denominator = c * b;
        break;
      default:
        fprintf(stderr, "invalid operator.\n"); // si no se encontro un operador soportado, imprimir un error
        exit(1);
      }
      // impresion en formato a/b y como decimal.
      printf("%d/%d%c%d/%d = ", a, b, op, c, d);
      printf("%d/%d = ", numerator, denominator);
      printf("%f\n", (float)(numerator) / (denominator));
    }
    else {
      // si los tipos de argumento no coinciden, imprimir un error
      fprintf(stderr, "invalid arguments.\n");
      return 1;
    }
  }
}
