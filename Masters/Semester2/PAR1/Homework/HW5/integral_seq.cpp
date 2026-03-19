#define _USE_MATH_DEFINES

#include <iostream>
#include <chrono>
#include <iomanip>
#include <cmath>

double f(double x)
{
  return 4.0 / (1.0 + x * x);
}

double integral_midpoint(double (*f)(double), int a, int b, long N)
{
  if (b == a)
    return 0;
  if (b < a)
    return -integral_midpoint(f, b, a, N);

  double step = (b - a) / static_cast<double>(N);
  double area = 0.0;
  double curr_mark = a;
  for (long i = 0; i < N; i++)
  {
    double mid = curr_mark + step / 2;
    area += step * f(mid);
    curr_mark += step;
  }
  return area;
}

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    std::cerr << "Invalid argument count. Usage: " << argv[0] << " <N>\n";
    return 1;
  }

  long N = std::atol(argv[1]);
  if (N <= 0)
  {
    std::cerr << "Partition size must be positive.\n";
    return 1;
  }

  constexpr int a = 0, b = 1;

  auto ti = std::chrono::steady_clock::now();
  double area = integral_midpoint(f, a, b, N);
  auto tf = std::chrono::steady_clock::now();

  std::chrono::duration<double> t = tf - ti;
  double time = t.count();

  std::cout << "===== SEQUENTIAL =====\n";
  std::cout << "Execution time: " << time * 1000 << " ms" << std::endl;
  std::cout << "Area: " << std::setprecision(15) << area << std::endl;
  std::cout << "Error: " << std::setprecision(15) << fabs(area - M_PI) << std::endl;
}