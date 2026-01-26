#include "../../cpp-utils/files/txt/input.hpp"
#include "../../cpp-utils/numeric/interpolate/lagrange.hpp"

#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>

using namespace interp;
using namespace file_input;

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cout << "Invalid argument count.\nUsage: " << argv[0] << "x.txt y.txt"
              << std::endl;
  }
  std::vector<double> x = readFile<double>(argv[1]);
  std::vector<double> y = readFile<double>(argv[2]);

  if (!std::filesystem::exists("ex/p3d")) {
    std::filesystem::create_directories("ex/p3d");
  }

  std::ofstream outFile("ex/p3d/coeffs.csv");
  std::vector<double> l = lagrange_coeffs(x, y);

  for (double c : l) {
    outFile << c << "\n";
  }

  outFile.close();
}
