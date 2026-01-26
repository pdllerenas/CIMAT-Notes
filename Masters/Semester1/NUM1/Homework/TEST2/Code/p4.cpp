#include "../../cpp-utils/numeric/solvers/Broyden.hpp"
#include <filesystem>
#include <fstream>
#include <vector>

// part a system of non-linea equations
auto f1 = [](const std::vector<double> &x) {
  return std::exp(-(x[0] * x[0] + x[1] * x[1])) * (1 - 2 * x[0] * x[0]);
};
auto f2 = [](const std::vector<double> &x) {
  return -2 * x[0] * x[1] * std::exp(-(x[0] * x[0] + x[1] * x[1]));
};

int main() {
  std::vector<double> xa = {-0.5, -0.5};

  std::vector<std::function<double(const std::vector<double> &)>> fa{f1, f2};

  if (!std::filesystem::exists("ex/p4")) {
    std::filesystem::create_directories("ex/p4");
  }
  std::ofstream file3("ex/p4/f.csv");

  std::vector<double> root =
      solvers::broyden::solve(fa, xa, 1e-12, 20000, &file3);
  return 0;
}
