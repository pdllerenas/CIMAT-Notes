#include "../../cpp-utils/files/txt/input.hpp"
#include "../../cpp-utils/numeric/interpolate/lagrange.hpp"

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

  if (!std::filesystem::exists("ex/lagrange")) {
    std::filesystem::create_directories("ex/lagrange");
  }

  std::ofstream outFile("ex/lagrange/table.csv");

  outFile << "z," << "P(z)\n";
  for (double zi : z) {
    // todo: return lagrange basis, so they are not calculated from scratch
    // every call
    double t = lagrange(x, y, zi);
    outFile << zi << "," << t << "\n";
  }

	outFile.close();
}
