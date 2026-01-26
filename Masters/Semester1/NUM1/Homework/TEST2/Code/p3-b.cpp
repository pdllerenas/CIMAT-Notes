#include "../../cpp-utils/files/txt/input.hpp"
#include "../../cpp-utils/numeric/interpolate/mse.hpp"
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

using namespace interp::mse;
using namespace file_input;

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "Invalid argument count. Usage: " << argv[0]
              << "<t.txt> <c.txt>\n";
    return 1;
  }
  std::vector<double> x = readFile<double>(argv[1]);
  std::vector<double> y = readFile<double>(argv[2]);

  std::vector<double> coeff;

  coeff = logistic_linearized(x, y, 0.0);

  std::cout << "coefficients:" << std::endl;
  for (double c : coeff) {
    std::cout << c << std::endl;
  }
}
