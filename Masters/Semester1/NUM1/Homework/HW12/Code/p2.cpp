#include "../../cpp-utils/numeric/differential/ODE.hpp"
#include "../../cpp-utils/numeric/integration/gaussian.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

// part a system of non-linea equations
auto f = [](double t, double y) { return sqrt(1 + t * t * t); };
auto f2 = [](double t) { return sqrt(1 + t * t * t); };

int main() {
  if (!std::filesystem::exists("ex/p2")) {
    std::filesystem::create_directories("ex/p2");
  }
  std::ofstream file1("ex/p2/rk4.csv");
  std::ofstream file2("ex/p2/taylor2.csv");
  std::ofstream file3("ex/p2/heun.csv");
  std::ofstream file4("ex/p2/euler.csv");

  double I = integration::gaussian::quadrature3(0.0, 2.0, f2);
  std::cout << "Gaussian Quadrature 3 result: " << std::endl;
  std::cout << I << std::endl;

  std::vector<std::pair<double, double>> res =
      differential::rk4(f, 0.0, 2.0, 10, 0.0);
  file1 << "x,y" << std::endl;
  for (auto r : res) {
    file1 << get<0>(r) << "," << get<1>(r) << std::endl;
  }

  file4 << "x,y" << std::endl;
  std::vector<std::pair<double, double>> res2 =
      differential::euler(f, 0.0, 2.0, 10, 0.0);
  for (auto r : res2) {
    file4 << get<0>(r) << "," << get<1>(r) << std::endl;
  }

  file2 << "x,y" << std::endl;
  std::vector<std::pair<double, double>> res3 =
      differential::taylor2(f, 0.0, 2.0, 10, 0.0);
  for (auto r : res3) {
    file2 << get<0>(r) << "," << get<1>(r) << std::endl;
  }

  file3 << "x,y" << std::endl;
  std::vector<std::pair<double, double>> res4 =
      differential::heun(f, 0.0, 2.0, 10, 0.0);
  for (auto r : res4) {
    file3 << get<0>(r) << "," << get<1>(r) << std::endl;
  }
}
