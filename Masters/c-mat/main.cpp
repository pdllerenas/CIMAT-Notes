#include <cmath>
#include <interpolate/taylor.hpp>
#include <interpolate/lagrange.hpp>
#include <interpolate/neville.hpp>
#include <iostream>

int main() {
  using namespace interp;
  double *df = (double *)malloc(4 * sizeof(double));
	df[0] = 1; df[1] = 0; df[2] = 0; df[3] = 0;

  double approx = taylor(0.5, 0.5, df, 4);
  double exact = 0.5;


	double *x = (double *)malloc(10 * sizeof(double));
	double *y = (double *)malloc(10 * sizeof(double));

	for (int i = 0; i < 10; i++) {
		x[i] = i;
		y[i] = i;
	}


	double l = lagrange<double>(x, y, 0.5, 10);

	double **Q = neville(0.5, x, y, 10);

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j <= i; j++) {
			std::cout << Q[i][j] << " ";
		}
		std::cout << "\n";
	}

	std::cout << "l = " << l << std::endl;

  std::cout << "Approximation: " << approx << '\n';
  std::cout << "True value:    " << exact << '\n';
  std::cout << "Error:         " << std::abs(approx - exact) << '\n';
}
