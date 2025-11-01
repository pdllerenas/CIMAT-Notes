#include "../../cpp-utils/files/txt/input.hpp"
#include "../../cpp-utils/numeric/interpolate/hermite.hpp"

#include <cmath>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>

using namespace interp;
using namespace file_input;

int main(int argc, char *argv[]) {
  if (argc != 5) {
    std::cout << "Invalid argument count.\nUsage: " << argv[0]
              << "x0 x.txt fx.txt dfx.txt" << std::endl;
  }
  double x0 = std::stod(argv[1]);
  std::vector<double> x = readFile<double>(argv[2]);
  std::vector<double> fx = readFile<double>(argv[3]);
  std::vector<double> dfx = readFile<double>(argv[4]);

  if (!std::filesystem::exists("ex/hermite")) {
    std::filesystem::create_directories("ex/hermite");
  }

  std::ofstream outFile("ex/hermite/coeff.csv");

  std::vector<double> coeff = hermite::polynomial(x, fx, dfx);
	for (double c : coeff) {
		outFile << c << std::endl;
	}
  double result = hermite::eval(x0, x, coeff);
  std::cout << "f(" << x0 << ") = " << std::format("{}", result) << std::endl;
  std::cout << "sin(" << x0 << ") = " << std::format("{}", std::sin(x0)) << std::endl;
  std::cout << "abs_err = " << std::format("{}", std::abs(std::sin(x0) - result)) << std::endl;
}
