#include "../../cpp-utils/files/txt/input.hpp"
#include "../../cpp-utils/numeric/interpolate/lagrange.hpp"
#include "../../cpp-utils/numeric/interpolate/neville.hpp"
#include "../../cpp-utils/numeric/interpolate/newton.hpp"

#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>

using namespace interp;
using namespace file_input;

int main(int argc, char *argv[]) {
  if (argc != 4) {
    std::cout << "Invalid argument count.\nUsage: " << argv[0]
              << "z.txt x.txt y.txt" << std::endl;
  }
  std::vector<double> z = readFile<double>(argv[1]);
  std::vector<double> x = readFile<double>(argv[2]);
  std::vector<double> y = readFile<double>(argv[3]);

  if (!std::filesystem::exists("ex/six")) {
    std::filesystem::create_directories("ex/six");
  }

  std::ofstream outFile("ex/six/table.csv");

  outFile << "z,I. Lag,I. Nev,I DD\n";
  std::vector coeff = divided_difference(x, y);
  for (double zi : z) {
    double l = lagrange(x, y, zi);
    double n = neville(zi, x, y);
    double d = evaluate(zi, x, coeff);
    outFile << zi << "," << l << "," << n << "," << d << "\n";
  }

  outFile.close();
}
