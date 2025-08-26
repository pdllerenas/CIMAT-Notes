#include <stdio.h>

// recibe un punto x real y un numero entero de terminos
// para aproximar e^(-x^2)
double taylor_exp(double x, unsigned int terms) {
  // inicializamos el resultado como 0
  // n_x_squared es -x^2, que usaremos para calcular
  // el siguiente termino de la serie,
  // notando que si el termino n-1 es
  // t_{n-1} = (-1)^(n-1)x^(2(n-1))/(n-1)!,
  // entonces el termino n es
  // t_{n} = t_{n-1} * (-1)x^2/n.
  double result = 0, n_x_squared = -x * x;
  double curr_term = 1; // el primer termino de la serie es 1 para cualquier x
  for (int i = 1; i < terms; i++) {
    result += curr_term;
    curr_term *= n_x_squared / i; // actualizamos el valor siguiendo el algoritmo mencionado anteriormente
  }
  return result;
}

int main() {
  double x;
  unsigned int terms;
  printf("Enter a point x:\n");
  scanf("%lf", &x);
  printf("Enter number of terms to approximate exp(-x^2):\n");
  scanf("%ud", &terms);
  printf("%lf\n", taylor_exp(x, terms));
}
