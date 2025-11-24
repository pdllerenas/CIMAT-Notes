#include "../../cpp-utils/numeric/differential/ODE.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <numbers>
#include <vector>

// part a system of non-linea equations
auto f = [](double t, double y) { return y - t * t + 1; };

int main() {
  if (!std::filesystem::exists("ex/p1")) {
    std::filesystem::create_directories("ex/p1");
  }
  std::ofstream file1("ex/p1/f1.txt");
  std::ofstream file2("ex/p1/f2.txt");

  std::vector<std::tuple<double, double>> res =
      differential::rk4(f, 0.0, 2.0, 10, 0.5);
  for (auto r : res) {
    std::cout << get<0>(r) << ", " << get<1>(r) << std::endl;
  }
}
