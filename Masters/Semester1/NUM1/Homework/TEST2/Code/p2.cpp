#include "../../cpp-utils/numeric/differential/ODE.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

// part a system of non-linea equations
auto f = [](double t, const std::vector<double> &x) {
  return std::vector<double>{x[1],
                             std::cos(t) + 2*x[1] - x[0]};
};

int main() {
  if (!std::filesystem::exists("ex/p2")) {
    std::filesystem::create_directories("ex/p2");
  }
  std::ofstream file1("ex/p2/rk4.csv");
  std::ofstream file2("ex/p2/taylor2.csv");
  std::ofstream file3("ex/p2/heun.csv");
  std::ofstream file4("ex/p2/euler.csv");

  std::vector<double> alpha = {1.5, 1.0};

	constexpr double a = 0.0;
	constexpr double b = 2.0;
	constexpr size_t N = 100;


  file1 << "t,u1,u2" << std::endl;
  matrix<double> res = differential::rk4(f, a, b, N, alpha);
  file1 << res << std::endl;

  file2 << "t,x,y" << std::endl;
  matrix<double> res2 = differential::taylor2(f, a, b, N, alpha);
  file2 << res2 << std::endl;

  file3 << "t,x,y" << std::endl;
  matrix<double> res3 = differential::heun(f, a, b, N, alpha);
  file3 << res3 << std::endl;

  file4 << "t,x,y" << std::endl;
  matrix<double> res4 = differential::euler(f, a, b, N, alpha);
  file4 << res4 << std::endl;
}
