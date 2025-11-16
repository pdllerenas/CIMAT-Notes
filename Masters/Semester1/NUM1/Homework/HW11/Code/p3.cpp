#include "../../cpp-utils/numeric/solvers/Broyden.hpp"
#include "../../cpp-utils/numeric/solvers/FixedPoint.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <numbers>
#include <vector>

// part a system of non-linea equations
auto f1 = [](const std::vector<double> &x) { return x[0] + x[1] - 3; };
auto f2 = [](const std::vector<double> &x) {
  return x[0] * x[0] + x[1] * x[1] - 9;
};

// part b system of non-linear equations
auto h1 = [](const std::vector<double> &x) {
  return 3 * x[0] - std::cos(x[1] * x[2]) - 0.5;
};
auto h2 = [](const std::vector<double> &x) {
  return x[0] * x[0] - 81 * (x[1] + 0.1) * (x[1] + 0.1) + std::sin(x[2]) + 1.06;
};
auto h3 = [](const std::vector<double> &x) {
  return std::exp(-x[0] * x[1]) + 20 * x[2] + (10 * std::numbers::pi - 3) / 3;
};

auto g1 = [](const std::vector<double> &x) { return 3 - x[1]; };
auto g2 = [](const std::vector<double> &x) {
  return std::sqrt(9 - x[0] * x[0]);
};

double k1(const std::vector<double> &x) {
  return std::cos(x[1] * x[2]) / 3.0 + 1.0 / 6.0;
}

double k2(const std::vector<double> &x) {
  return sqrt(x[0] * x[0] + std::sin(x[2]) + 1.06) / 9.0 - 0.1;
}

double k3(const std::vector<double> &x) {
  return -std::exp(-x[0] * x[1]) / 20.0 - (10 * std::numbers::pi - 3) / 60.0;
}

int main() {
  std::vector<double> xa = {1, 5};
  std::vector<double> xb = {0.1, 0.1, -0.1};

  std::vector<std::function<double(const std::vector<double> &)>> fa{f1, f2};
  std::vector<std::function<double(const std::vector<double> &)>> fb{h1, h2,
                                                                     h3};

  std::vector<std::function<double(const std::vector<double> &)>> ga{g1, g2};
  std::vector<std::function<double(const std::vector<double> &)>> gb{k1, k2,
                                                                     k3};
  if (!std::filesystem::exists("ex/fixed")) {
    std::filesystem::create_directories("ex/fixed");
  }
  std::ofstream file1("ex/fixed/f1.txt");
  std::ofstream file2("ex/fixed/f2.txt");

  std::vector<double> F_root_a =
      solvers::fixed_point::solve(ga, xa, 1e-9, 20000, &file1);
  std::vector<double> F_root_b =
      solvers::fixed_point::solve(gb, xb, 1e-12, 20000, &file2);

  if (!std::filesystem::exists("ex/broyden")) {
    std::filesystem::create_directories("ex/broyden");
  }
  std::ofstream file3("ex/broyden/f1.txt");
  std::ofstream file4("ex/broyden/f2.txt");

  std::vector<double> root_a =
      solvers::broyden::solve(fa, xa, 1e-9, 20000, &file3);
  std::vector<double> root_b =
      solvers::broyden::solve(fb, xb, 1e-12, 20000, &file4);
  return 0;
}
