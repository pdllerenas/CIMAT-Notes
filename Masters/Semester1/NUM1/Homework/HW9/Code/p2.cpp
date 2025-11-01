#include "../../cpp-utils/files/txt/input.hpp"
#include "../../cpp-utils/numeric/interpolate/spline.hpp"

#include <cmath>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>

using namespace interp;
using namespace file_input;

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cout << "Invalid argument count.\nUsage: " << argv[0]
              << "x.txt fx.txt" << std::endl;
  }
  std::vector<double> x = readFile<double>(argv[1]);
  std::vector<double> fx = readFile<double>(argv[2]);

  if (!std::filesystem::exists("ex/spline-nat")) {
    std::filesystem::create_directories("ex/spline-nat");
  }

  std::ofstream outFile("ex/spline-nat/table.csv");

  std::vector<std::tuple<double, double, double, double>> coeff =
      spline::natural(x, fx);
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
