#include "../../cpp-utils/files/txt/input.hpp"
#include "../../cpp-utils/numeric/integration/gaussian.hpp"
#include "../../cpp-utils/numeric/integration/newton-cotes.hpp"

#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <numbers>

using namespace file_input;
using namespace integration::newtoncotes;
using namespace integration::gaussian;

int main(int argc, char *argv[]) {
  if (!std::filesystem::exists("ex/xexp")) {
    std::filesystem::create_directories("ex/xexp");
  }

  std::ofstream outFile("ex/xexp/closed.csv");

  outFile << "n,value\n";

  double A0 = closed::trapezoid(0.0, 1.0,
                                [](double x) { return x * x * std::exp(-x); });
  outFile << "1," << A0 << '\n';
  double A1 =
      closed::simpson(0.0, 1.0, [](double x) { return x * x * std::exp(-x); });
  outFile << "2," << A1 << '\n';
  double A2 = closed::simpson38(0.0, 1.0,
                                [](double x) { return x * x * std::exp(-x); });
  outFile << "3," << A2 << '\n';
  double A3 = closed::five_point(0.0, 1.0,
                                 [](double x) { return x * x * std::exp(-x); });
  outFile << "4," << A3 << '\n';

  std::ofstream outFile2("ex/xexp/open.csv");

  outFile2 << "n,value\n";

  A0 = open::midpoint(0.0, 1.0, [](double x) { return x * x * std::exp(-x); });
  outFile2 << "0," << A0 << '\n';
  A1 = open::n2(0.0, 1.0, [](double x) { return x * x * std::exp(-x); });
  outFile2 << "1," << A1 << '\n';
  A2 = open::simpson38(0.0, 1.0, [](double x) { return x * x * std::exp(-x); });
  outFile2 << "2," << A2 << '\n';
  A3 =
      open::five_point(0.0, 1.0, [](double x) { return x * x * std::exp(-x); });
  outFile2 << "3," << A3 << '\n';

  std::ofstream outFile3("ex/xexp/quadrature.csv");

  outFile3 << "n,value\n";

  A0 = quadrature(0.0, 1.0, [](double x) { return x * x * std::exp(-x); }, 2);
  outFile3 << "2," << A0 << '\n';
  A1 = quadrature(0.0, 1.0, [](double x) { return x * x * std::exp(-x); }, 3);
  outFile3 << "3," << A1 << '\n';
  A2 = quadrature(0.0, 1.0, [](double x) { return x * x * std::exp(-x); }, 4);
  outFile3 << "4," << A2 << '\n';
  A3 = quadrature(0.0, 1.0, [](double x) { return x * x * std::exp(-x); }, 5);
  outFile3 << "5," << A3 << '\n';
}
