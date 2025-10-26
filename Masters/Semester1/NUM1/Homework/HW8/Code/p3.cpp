#include "../../cpp-utils/files/txt/input.hpp"
#include "../../cpp-utils/numeric/interpolate/neville.hpp"

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

  if (!std::filesystem::exists("ex/neville")) {
    std::filesystem::create_directories("ex/neville");
  }

  std::ofstream outFile("ex/neville/table.csv");

  outFile << "z,P(z),|P(Z) - f(z)|\n";
  for (double zi : z) {
    double t = neville(zi, x, y);
    outFile << zi << "," << t << "," << std::abs(t - std::log(zi)) << "\n";
  }

	outFile.close();
}
