#include "../../cpp-utils/numeric/differential/ODE.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

// part a system of non-linea equations
auto f = [](double t, const std::vector<double> &x) {
  return std::vector<double>{0.4 * x[0] - 0.018 * x[0] * x[1],
                             -0.8 * x[1] + 0.023 * x[0] * x[1]};
};

int main() {
  if (!std::filesystem::exists("ex/p3")) {
    std::filesystem::create_directories("ex/p3");
  }
  std::ofstream file1("ex/p3/rk4.csv");
  std::ofstream file2("ex/p3/taylor2.csv");
  std::ofstream file3("ex/p3/heun.csv");
  std::ofstream file4("ex/p3/euler.csv");

  std::vector<double> alpha = {30, 4};

  file1 << "year,rabbit,lynx" << std::endl;
  matrix<double> res = differential::rk4(f, 0.0, 25.0, 100, alpha);
  file1 << res << std::endl;

  file2 << "year,rabbit,lynx" << std::endl;
  matrix<double> res2 = differential::taylor2(f, 0.0, 25.0, 100, alpha);
  file2 << res2 << std::endl;

  file3 << "year,rabbit,lynx" << std::endl;
  matrix<double> res3 = differential::heun(f, 0.0, 25.0, 100, alpha);
  file3 << res3 << std::endl;

  file4 << "year,rabbit,lynx" << std::endl;
  matrix<double> res4 = differential::euler(f, 0.0, 25.0, 100, alpha);
  file4 << res4 << std::endl;
}
