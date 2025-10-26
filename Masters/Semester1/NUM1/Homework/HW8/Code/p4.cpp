#include "../../cpp-utils/files/txt/input.hpp"
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

  if (!std::filesystem::exists("ex/newton")) {
    std::filesystem::create_directories("ex/newton");
  }

  std::ofstream outFile("ex/newton/table.csv");

  outFile << "z,P(z)\n";
  std::vector coeff = divided_difference(x, y);
  for (double zi : z) {
    double ft = evaluate(zi, x, coeff);
    outFile << zi << "," << ft << "\n";
  }

  outFile.close();
}
