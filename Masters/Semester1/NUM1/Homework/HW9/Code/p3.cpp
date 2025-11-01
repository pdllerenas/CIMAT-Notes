#include "../../cpp-utils/files/txt/input.hpp"
#include "../../cpp-utils/numeric/interpolate/spline.hpp"

#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>

using namespace interp;
using namespace file_input;

int main(int argc, char *argv[]) {
  if (argc != 6) {
    std::cout << "Invalid argument count.\nUsage: " << argv[0]
              << "x.txt fx.txt FP0 FPN outfile" << std::endl;
  }
  std::vector<double> x = readFile<double>(argv[1]);
  std::vector<double> fx = readFile<double>(argv[2]);
  double FP0 = std::stod(argv[3]);
  double FPN = std::stod(argv[4]);

  if (!std::filesystem::exists("ex/spline-cond")) {
    std::filesystem::create_directories("ex/spline-cond");
  }

  std::ofstream outFile(argv[5]);

  std::vector<std::tuple<double, double, double, double>> coeff =
      spline::conditioned(x, fx, FP0, FPN);
  int i = 0;
  outFile << "i," << "x," << "a," << "b," << "c," << "d" << std::endl;
  for (auto c : coeff) {
    outFile << i << ",";
    outFile << x[i++] << ",";
    outFile << std::get<0>(c) << ",";
    outFile << std::get<1>(c) << ",";
    outFile << std::get<2>(c) << ",";
    outFile << std::get<3>(c) << std::endl;
  }
}
