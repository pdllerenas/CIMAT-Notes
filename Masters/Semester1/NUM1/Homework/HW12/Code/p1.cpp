#include "../../cpp-utils/numeric/differential/ODE.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <numbers>
#include <vector>

// part a system of non-linea equations
auto f = [](double t, double y) { return y; };

int main() {
  if (!std::filesystem::exists("ex/p1")) {
    std::filesystem::create_directories("ex/p1");
  }
  std::ofstream file1("ex/p1/rk4.csv");
  std::ofstream file2("ex/p1/taylor2.csv");
  std::ofstream file3("ex/p1/heun.csv");
  std::ofstream file4("ex/p1/euler.csv");

  std::vector<std::pair<double, double>> res =
      differential::rk4(f, 0.0, 4.0, 100, 1.0);
  file1 << "x,y" << std::endl;
  for (auto r : res) {
    file1 << get<0>(r) << "," << get<1>(r) << std::endl;
  }

  file4 << "x,y" << std::endl;
  std::vector<std::pair<double, double>> res2 =
      differential::euler(f, 0.0, 4.0, 100, 1.0);
  for (auto r : res2) {
    file4 << get<0>(r) << "," << get<1>(r) << std::endl;
  }

  file2 << "x,y" << std::endl;
  std::vector<std::pair<double, double>> res3 =
      differential::taylor2(f, 0.0, 4.0, 100, 1.0);
  for (auto r : res3) {
    file2 << get<0>(r) << "," << get<1>(r) << std::endl;
  }

  file3 << "x,y" << std::endl;
  std::vector<std::pair<double, double>> res4 =
      differential::heun(f, 0.0, 4.0, 100, 1.0);
  for (auto r : res4) {
    file3 << get<0>(r) << "," << get<1>(r) << std::endl;
  }
}
