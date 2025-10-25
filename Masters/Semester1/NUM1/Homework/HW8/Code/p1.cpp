#include "../../cpp-utils/files/txt/input.hpp"
#include "../../cpp-utils/numeric/interpolate/taylor.hpp"

#include <cmath>
#include <fstream>
#include <iostream>
#include <filesystem>

using namespace interp;
using namespace file_input;

int main(int argc, char *argv[]) {
  if (argc != 5) {
    std::cout << "Invalid argument count.\nUsage: " << argv[0]
              << " x0 n.txt z.txt fx_0.txt" << std::endl;
  }
  double x0 = std::stod(argv[1]);
  std::vector<int> n = readFile<int>(argv[2]);
  std::vector<double> z = readFile<double>(argv[3]);
  std::vector<double> fx = readFile<double>(argv[4]);

	if (!std::filesystem::exists("ex/taylor")) {
        std::filesystem::create_directories("ex/taylor");
    }

  std::ofstream outFile("ex/taylor/table.csv");

  if (!outFile) {
    std::cerr << "Error: Could not open file." << std::endl;
    return 1;
  }
  outFile << "n," << "z," << "P_n(z)," << "|P_n(z) - f(z)|\n";

  for (std::size_t j = 0; j < n.size(); j++) {
    for (std::size_t i = 0; i < z.size(); i++) {
      double t = taylor(z[i], x0, fx, n[j]);
      outFile << n[j] << "," << z[i] << "," << t << "," << std::abs(std::exp(z[i]) - t) << "\n";
    }
  }
}
